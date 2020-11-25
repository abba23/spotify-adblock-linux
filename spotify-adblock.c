#define _GNU_SOURCE

#include <dlfcn.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>

#include "include/capi/cef_urlrequest_capi.h"

#include "blacklist.h"
#include "whitelist.h"

#define INIT_REAL_FUNCTION(name) \
    do { \
        real_##name = dlsym(RTLD_NEXT, #name); \
        if (!real_##name) { \
            fprintf(stderr, "dlsym (%s): %s\n", #name, dlerror()); \
        } \
    } while (0)

typeof(getaddrinfo) *real_getaddrinfo = NULL;
typeof(cef_urlrequest_create) *real_cef_urlrequest_create = NULL;

const ssize_t whitelist_size = sizeof(whitelist) / sizeof(whitelist[0]);
const ssize_t blacklist_size = sizeof(blacklist) / sizeof(blacklist[0]);

void __attribute__((constructor)) init(void) {
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

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    if (listed(node, whitelist, whitelist_size)) {
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
    if (listed(url, blacklist, blacklist_size)) {
        printf("[-] cef_urlrequest_create:\t%s\n", url);
        return NULL;
    }
    printf("[+] cef_urlrequest_create:\t%s\n", url);
    return real_cef_urlrequest_create(request, client, request_context);
}
