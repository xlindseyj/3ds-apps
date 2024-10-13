#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "ui.h"

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    displayMessage("Initializing...");

    if (initNetworking() != 0) {
        displayMessage("Network initialization failed. Press START to exit.");
        waitForKeyPress(KEY_START);
        cleanupUI();
        return -1;
    }

    displayMessage("Press A to connect to the Tailscale network.");
    waitForKeyPress(KEY_A);

    const char* api_key = getenv("TAILSCALE_API_KEY");
    if (!api_key) {
        displayMessage("Error: TAILSCALE_API_KEY not set. Press START to exit.");
        waitForKeyPress(KEY_START);
        cleanupNetworking();
        cleanupUI();
        return -1;
    }

    displayMessage("Connecting to Tailscale...");
    if (connectToVPN(api_key) != 0) {
        displayMessage("Failed to connect. Press START to exit.");
        waitForKeyPress(KEY_START);
    } else {
        displayMessage("Connected to Tailscale! Press START to exit.");
        waitForKeyPress(KEY_START);
    }

    cleanupNetworking();
    cleanupUI();
    return 0;
}
