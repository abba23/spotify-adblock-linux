#define _GNU_SOURCE

#include <dlfcn.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <cyaml/cyaml.h>

#include "include/capi/cef_urlrequest_capi.h"

#include "config.h"

#define INIT_REAL_FUNCTION(name) \
    do { \
        real_##name = dlsym(RTLD_NEXT, #name); \
        if (!real_##name) { \
            fprintf(stderr, "dlsym (%s): %s\n", #name, dlerror()); \
        } \
    } while (0)

// enable early debug by setting env var DEBUG=1
int early_debug = FALSE;

//typeof(main) *real_main = NULL; // error: 'main' undeclared here (not in a function)
//int (*real_main)(int argc, char **argv); // TODO better?
//static int (*real_main)(int, char **, char **); // https://gist.github.com/apsun/1e144bf7639b22ff0097171fa0f8c6b1

typedef int (*main_t)(int, char **, char **);
static main_t real_main;

// TODO why does typeof() work here?
typeof(getaddrinfo) *real_getaddrinfo = NULL;
typeof(cef_urlrequest_create) *real_cef_urlrequest_create = NULL;

//const ssize_t whitelist_size = sizeof(whitelist) / sizeof(whitelist[0]);
//const ssize_t blacklist_size = sizeof(blacklist) / sizeof(blacklist[0]);

void __attribute__((constructor)) init(void) {
    ////////INIT_REAL_FUNCTION(main);
    INIT_REAL_FUNCTION(getaddrinfo);
    INIT_REAL_FUNCTION(cef_urlrequest_create);
}

bool listed(const char *item, const char *list[], ssize_t list_size) {
    for (ssize_t i = 0; i < list_size; i++) {
        if (!fnmatch(list[i], item, 0)) {
            return true;
        }
    }
    return false;
}

/* FIXME
when we wrap main() with wrap_main()
we no longer see errors from ld

TODO ask on stackoverflow.com
__libc_start_main
wrap main function
symbol lookup error: /lib/spotify-adblock.so: undefined symbol: cef_string_userfree_utf16_free
missing LDLIBS flag -lcef

for example:
when spotify-adblock.so is not linked against libcef.so
then usually we get the error
symbol lookup error: /lib/spotify-adblock.so: undefined symbol: cef_string_userfree_utf16_free

but when main() is wrapped
we must run our program in a debugger to see that error:
gdb echo
(gdb) set environment LD_PRELOAD /lib/spotify-adblock.so
(gdb) start
Temporary breakpoint 1 at 0x408630
Starting program: /bin/echo
/bin/bash: symbol lookup error: /lib/spotify-adblock.so: undefined symbol: cef_string_userfree_utf16_free
During startup program exited with code 127.
(gdb)
-> error: in LDLIBS, flag -lcef is missing

example 2:
gdb echo
(gdb) set environment LD_PRELOAD /lib/spotify-adblock.so
(gdb) start
Temporary breakpoint 1 at 0x408630
Starting program: /bin/echo
/bin/bash: error while loading shared libraries: libgobject-2.0.so.0: cannot open shared object file: No such file or directory
During startup program exited with code 127.
(gdb)
-> error: LD_LIBRARY_PATH is not set (NixOS)

... but such errors should be visible without a debugger
*/

config_t *config = NULL;
char *config_file = NULL; // null: no config file was loaded

int wrap_main(int argc, char **argv, char **envp) {

    //config_t *config = NULL;
    //char *config_file = NULL; // null: no config file was loaded

    if (early_debug) printf("wrap main: load_config\n");
    load_config("spotify-adblock-linux", &config, &config_file);

    if (config->debug == TRUE) {
        printf("load_config done\n");
        printf("config_file = %s\n", config_file);
        // print config
        printf("config:\n");
        printf("```yaml\n");
        printf("debug: %s\n", (config->debug == TRUE ? "true" : "false"));
        printf("whitelist:\n");
        for (unsigned i = 0; i < config->whitelist_count; i++) {
            printf("  - %s\n", config->whitelist[i]);
        }
        printf("blacklist:\n");
        for (unsigned i = 0; i < config->blacklist_count; i++) {
            printf("  - %s\n", config->blacklist[i]);
        }
        printf("```\n");
    }

    int main_res = real_main(argc, argv, envp);
    if (config->debug) printf("main() returned %d\n", main_res);

    // cleanup
    // free config
    if (config_file != NULL) {
        // config was loaded by cyaml
        cyaml_free(&cyaml_config, &top_schema, config, 0);
    }
    else {
        // config was set manually
        free(config->whitelist);
        free(config->blacklist);
        free(config);
    }

    return main_res;
}

// wrap __libc_start_main: replace real_main with wrap_main
int __libc_start_main(
    main_t main, int argc, char **argv,
    //int (*init)(int, char **, char **),
    // void (*init) (void),
    main_t init,
    void (*fini)(void), void (*rtld_fini)(void), void *stack_end
) {
    if (getenv("DEBUG") != NULL) early_debug = TRUE;
    if (early_debug) printf("wrap __libc_start_main\n");

    static int (*real___libc_start_main)() = NULL;
    if (!real___libc_start_main) {
        if (early_debug) printf("real___libc_start_main = %p (falsy)\n", real___libc_start_main);
        char *error;
        real___libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
        if (early_debug) printf("real___libc_start_main = %p\n", real___libc_start_main);
        if ((error = dlerror()) != NULL) {
            printf("%s\n", error);
            exit(1);
        }
    }
    real_main = main;
    return real___libc_start_main(wrap_main, argc, argv, init, fini, rtld_fini, stack_end);

/*
    real_main = main;
    typeof(&__libc_start_main) real___libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
    if (early_debug) printf("wrap __libc_start_main: done\n");
    return real___libc_start_main(wrap_main, argc, argv, init, fini, rtld_fini, stack_end);
*/

}



int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    //if (listed(node, whitelist, whitelist_size)) {
    if (listed(node, (const char **) config->whitelist, config->whitelist_count)) {
        printf("[+] getaddrinfo:\t\t%s\n", node);
        return real_getaddrinfo(node, service, hints, res);
    }
    printf("[-] getaddrinfo:\t\t%s\n", node);
    return EAI_FAIL;
}

cef_urlrequest_t* cef_urlrequest_create(struct _cef_request_t* request, struct _cef_urlrequest_client_t* client, struct _cef_request_context_t* request_context) {
    cef_string_userfree_utf16_t url_utf16 = request->get_url(request);
    char url[url_utf16->length + 1];
    url[url_utf16->length] = '\0';
    for (int i = 0; i < url_utf16->length; i++) url[i] = *(url_utf16->str + i);
    cef_string_userfree_utf16_free(url_utf16);
    //if (listed(url, blacklist, blacklist_size)) {
    if (listed(url, (const char **) config->blacklist, config->blacklist_count)) {
        printf("[-] cef_urlrequest_create:\t%s\n", url);
        return NULL;
    }
    printf("[+] cef_urlrequest_create:\t%s\n", url);
    return real_cef_urlrequest_create(request, client, request_context);
}
