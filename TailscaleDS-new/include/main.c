#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <citro3d.h>

// You'll need to implement or port these
#include "tailscale_client.h"
#include "ftp_server.h"
#include "ssh_server.h"
#include "ui_sprites.h" // This should contain sprite definitions from the example project

#define TAILSCALE_API_KEY "your_tailscale_api_key_here"
#define FTP_PORT 21
#define SSH_PORT 22

static volatile int running = 1;
static TailscaleClient* tailscale = NULL;
static FTPServer* ftp_server = NULL;
static SSHServer* ssh_server = NULL;

// UI states
typedef enum {
    STATE_INIT,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_FAILED,
    STATE_EXITING
} AppState;

static AppState current_state = STATE_INIT;

void cleanup() {
    running = 0;
    if (tailscale) tailscale_disconnect(tailscale);
    if (ftp_server) ftp_server_stop(ftp_server);
    if (ssh_server) ssh_server_stop(ssh_server);
}

int init_services() {
    // Initialize Tailscale client
    tailscale = tailscale_init(TAILSCALE_API_KEY);
    if (!tailscale) {
        return -1;
    }

    // Connect to Tailscale network
    if (tailscale_connect(tailscale) != 0) {
        return -1;
    }

    // Get Tailscale IP
    char tailscale_ip[INET_ADDRSTRLEN];
    tailscale_get_ip(tailscale, tailscale_ip, sizeof(tailscale_ip));

    // Initialize and start FTP server
    ftp_server = ftp_server_init(tailscale_ip, FTP_PORT);
    if (!ftp_server || ftp_server_start(ftp_server) != 0) {
        return -1;
    }

    // Initialize and start SSH server
    ssh_server = ssh_server_init(tailscale_ip, SSH_PORT);
    if (!ssh_server || ssh_server_start(ssh_server) != 0) {
        return -1;
    }

    return 0;
}

void draw_ui() {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_SceneBegin(GFX_TOP);

    // Clear the screen
    C2D_TargetClear(GFX_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));

    // Draw background sprite
    C2D_DrawSprite(&spriteBackground);

    // Draw text based on current state
    switch(current_state) {
        case STATE_INIT:
            C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 10.0f, 120.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f), "Press A to connect to Tailscale");
            break;
        case STATE_CONNECTING:
            C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 10.0f, 120.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f), "Connecting to Tailscale...");
            break;
        case STATE_CONNECTED:
            C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 10.0f, 120.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f), "Connected to Tailscale");
            C2D_DrawText(&g_staticText[1], C2D_AtBaseline | C2D_WithColor, 10.0f, 150.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f), "FTP and SSH servers running");
            break;
        case STATE_FAILED:
            C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 10.0f, 120.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f), "Connection failed");
            C2D_DrawText(&g_staticText[1], C2D_AtBaseline | C2D_WithColor, 10.0f, 150.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f), "Press SELECT to retry or START to exit");
            break;
        case STATE_EXITING:
            C2D_DrawText(&g_staticText[0], C2D_AtBaseline | C2D_WithColor, 10.0f, 120.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f), "Exiting...");
            break;
    }

    C3D_FrameEnd(0);
}

int main(int argc, char* argv[]) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Initialize sprites and text system
    init_sprites();
    init_text();

    while (aptMainLoop() && running) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        switch(current_state) {
            case STATE_INIT:
                if (kDown & KEY_A) {
                    current_state = STATE_CONNECTING;
                }
                break;
            case STATE_CONNECTING:
                if (init_services() == 0) {
                    current_state = STATE_CONNECTED;
                } else {
                    current_state = STATE_FAILED;
                }
                break;
            case STATE_CONNECTED:
                if (tailscale && !tailscale_is_connected(tailscale)) {
                    current_state = STATE_FAILED;
                }
                break;
            case STATE_FAILED:
                if (kDown & KEY_SELECT) {
                    current_state = STATE_CONNECTING;
                } else if (kDown & KEY_START) {
                    current_state = STATE_EXITING;
                    running = 0;
                }
                break;
            case STATE_EXITING:
                // Do nothing, we'll exit the loop
                break;
        }

        if (kDown & KEY_START && current_state != STATE_FAILED) {
            current_state = STATE_EXITING;
            running = 0;
        }

        draw_ui();

        gspWaitForVBlank();
    }

    cleanup();

    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}