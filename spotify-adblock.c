#define _GNU_SOURCE

#include <dlfcn.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "whitelist.h"

static typeof(getaddrinfo) *real_getaddrinfo = NULL;

static void init_real_getaddrinfo(void) {
    real_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
    if (!real_getaddrinfo) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
    }
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    int i;
    for (i = 0; i < sizeof(whitelist) / sizeof(whitelist[0]); i++) {
        if (fnmatch(whitelist[i], node, FNM_NOESCAPE) == 0) {
            printf("[+] %s\n", node);
            if (!real_getaddrinfo) {
                init_real_getaddrinfo();
            }
            return real_getaddrinfo(node, service, hints, res);
        }
    }
    printf("[-] %s\n", node);
    return -1;
}
