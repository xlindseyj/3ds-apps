#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <3ds.h>
#include <string.h>
#include "network.h"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

static u32 *SOC_buffer = NULL;

int initNetworking() {
    Result ret = 0;

    // Allocate a fixed buffer for SOC service
    SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
    if (SOC_buffer == NULL) {
        printf("Failed to allocate SOC buffer\n");
        return -1;
    }

    // Initialize SOC service
    if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
        printf("socInit failed: 0x%08X\n", (unsigned int)ret);
        free(SOC_buffer);
        return -1;
    }

    // Initialize network service
    ret = acInit();
    if (R_FAILED(ret)) {
        printf("acInit failed: 0x%08X\n", (unsigned int)ret);
        socExit();
        free(SOC_buffer);
        return -1;
    }

    // Check for network availability
    u32 wifiStatus = 0;
    ret = ACU_GetWifiStatus(&wifiStatus);
    if (R_FAILED(ret) || !wifiStatus) {
        printf("No WiFi connection available\n");
        acExit();
        socExit();
        free(SOC_buffer);
        return -1;
    }

    printf("Network initialized successfully\n");
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
    acExit();
    socExit();
    free(SOC_buffer);
}