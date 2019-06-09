#define _GNU_SOURCE

#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <fnmatch.h>
#include <netdb.h>
#include <stdio.h>

#include "whitelist.h"
#include "blacklist.h"

static typeof(getaddrinfo) *real_getaddrinfo = NULL;
static typeof(connect) *real_connect = NULL;

static void init_real_getaddrinfo(void) {
    real_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
    if (!real_getaddrinfo) {
        fprintf(stderr, "dlsym (getaddrinfo): %s\n", dlerror());
    }
}

static void init_real_connect(void) {
    real_connect = dlsym(RTLD_NEXT, "connect");
    if (!real_connect) {
        fprintf(stderr, "dlsym (connect): %s\n", dlerror());
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
    return EAI_FAIL;
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len) {
    const char *ip_address = inet_ntoa(((struct sockaddr_in *)address)->sin_addr);
    int i;
    for (i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
        if (!fnmatch(blacklist[i], ip_address, FNM_NOESCAPE)) {
            printf("[-] %s\n", ip_address);
            return ECONNREFUSED;
        }
    }
    printf("[+] %s\n", ip_address);
    if (!real_connect) {
        init_real_connect();
    }
    return real_connect(socket, address, address_len);
}
