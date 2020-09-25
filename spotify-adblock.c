#define _GNU_SOURCE

#include <curl/curl.h>
#include <dlfcn.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdarg.h>
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

static typeof(getaddrinfo) *real_getaddrinfo = NULL;
static typeof(curl_easy_setopt) *real_curl_easy_setopt = NULL;
static typeof(cef_urlrequest_create) *real_cef_urlrequest_create = NULL;

void __attribute__((constructor)) init(void) {
    INIT_REAL_FUNCTION(getaddrinfo);
    INIT_REAL_FUNCTION(curl_easy_setopt);
    INIT_REAL_FUNCTION(cef_urlrequest_create);
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    int i;
    for (i = 0; i < sizeof(whitelist) / sizeof(whitelist[0]); i++) {
        if (!fnmatch(whitelist[i], node, 0)) {
            printf("[+] getaddrinfo:\t\t%s\n", node);
            return real_getaddrinfo(node, service, hints, res);
        }
    }
    printf("[-] getaddrinfo:\t\t%s\n", node);
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
            if (!fnmatch(blacklist[i], url, 0)) {
                printf("[-] curl_easy_setopt:\t\t%s\n", url);
                return CURLE_OK;
            }
        }
        printf("[+] curl_easy_setopt:\t\t%s\n", url);
    }
    void *args = __builtin_apply_args();
    void *ret = __builtin_apply((void *)real_curl_easy_setopt, args, 500);
    __builtin_return(ret);
}

cef_urlrequest_t* cef_urlrequest_create(struct _cef_request_t* request, struct _cef_urlrequest_client_t* client, struct _cef_request_context_t* request_context) {
    cef_string_userfree_utf16_t url_utf16 = request->get_url(request);
    char url[url_utf16->length + 1];
    url[url_utf16->length] = '\0';
    for (int i = 0; i < url_utf16->length; i++) url[i] = *(url_utf16->str + i);
    cef_string_userfree_utf16_free(url_utf16);
    for (int i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
        if (!fnmatch(blacklist[i], url, 0)) {
            printf("[-] cef_urlrequest_create:\t%s\n", url);
            return NULL;
        }
    }
    printf("[+] cef_urlrequest_create:\t%s\n", url);
    return real_cef_urlrequest_create(request, client, request_context);
}
