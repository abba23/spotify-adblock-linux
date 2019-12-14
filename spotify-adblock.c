#define _GNU_SOURCE

#include <curl/curl.h>
#include <dlfcn.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>

#include "whitelist.h"
#include "blacklist.h"

static typeof(getaddrinfo) *real_getaddrinfo = NULL;
static typeof(curl_easy_setopt) *real_setopt = NULL;

static void init_real_getaddrinfo(void) {
    real_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
    if (!real_getaddrinfo) {
        fprintf(stderr, "dlsym (getaddrinfo): %s\n", dlerror());
    }
}

static void init_real_setopt(void) {
    real_setopt = dlsym(RTLD_NEXT, "curl_easy_setopt");
    if (!real_setopt) {
        fprintf(stderr, "dlsym (curl_easy_setopt): %s\n", dlerror());
    }
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    int i;
    for (i = 0; i < sizeof(whitelist) / sizeof(whitelist[0]); i++) {
        if (!fnmatch(whitelist[i], node, FNM_NOESCAPE)) {
            printf("[+] %s\n", node);
            if (!real_getaddrinfo) init_real_getaddrinfo();
            return real_getaddrinfo(node, service, hints, res);
        }
    }
    printf("[-] %s\n", node);
    return EAI_FAIL;
}

#undef curl_easy_setopt
CURLcode curl_easy_setopt(CURL *handle, CURLoption option, ...) {
    if (option == CURLOPT_URL) {
        va_list args;
        va_start(args, option);
        char *url = va_arg(args, char *);
        va_end(args);
        int i;
        for (i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
            if (!fnmatch(blacklist[i], url, FNM_NOESCAPE)) {
                printf("[-] %s\n", url);
                // destroy handle, so the request can never be attempted
                curl_easy_cleanup(handle);
                handle = NULL;
                return CURLE_OK;
            }
        }
        printf("[+] %s\n", url);
    }
    if (!real_setopt) init_real_setopt();
    void *args = __builtin_apply_args();
    void *ret = __builtin_apply((void *)real_setopt, args, 500);
    __builtin_return(ret);
}
