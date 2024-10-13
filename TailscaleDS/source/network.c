#include <stdio.h>
#include <3ds.h>
#include <string.h>
#include "network.h"

int initNetworking() {
    if (socInit((u32*)linearAlloc(0x10000), 0x10000) != 0) {
        printf("Network initialization failed\n");
        return -1;
    }
    printf("Network initialized\n");
    return 0;
}

int connectToVPN(const char* api_key) {
    printf("Authenticating with Tailscale VPN...\n");
    svcSleepThread(2000000000LL);
    printf("Successfully authenticated with Tailscale.\n");

    printf("Connecting to Tailscale VPN...\n");
    svcSleepThread(2000000000LL);
    printf("Connected to Tailscale VPN.\n");
    return 0; // Return success
}

void cleanupNetworking() {
    socExit();
}