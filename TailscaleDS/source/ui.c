#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include "ui.h"

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240
#define BOT_SCREEN_WIDTH  320
#define BOT_SCREEN_HEIGHT 240

static C2D_TextBuf g_staticBuf;
static C2D_Text g_staticText;

void initUI() {
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    g_staticBuf = C2D_TextBufNew(4096);
}

void displayMessage(const char* message, float progress) {
    C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT), C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_TargetClear(C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT), C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));

    C2D_TextBufClear(g_staticBuf);
    C2D_TextParse(&g_staticText, g_staticBuf, message);
    C2D_TextOptimize(&g_staticText);

    C2D_SceneBegin(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT));
    C2D_DrawText(&g_staticText, C2D_WithColor, (TOP_SCREEN_WIDTH - 200) / 2, (TOP_SCREEN_HEIGHT - 30) / 2, 0.5f, 0.5f, 0.5f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));

    // Draw progress bar
    if (progress >= 0 && progress <= 1) {
        C2D_DrawRectSolid(50, TOP_SCREEN_HEIGHT - 50, 0.5f, 300, 20, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
        C2D_DrawRectSolid(50, TOP_SCREEN_HEIGHT - 50, 0.5f, 300 * progress, 20, C2D_Color32(0x00, 0xFF, 0x00, 0xFF));
    }

    C2D_SceneBegin(C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT));
    C2D_DrawText(&g_staticText, C2D_WithColor, (BOT_SCREEN_WIDTH - 200) / 2, (BOT_SCREEN_HEIGHT - 30) / 2, 0.5f, 0.5f, 0.5f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));

    C2D_Flush();
    gfxFlushBuffers();
    gfxSwapBuffers();
}

void waitForKeyPress(u32 key) {
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & key) break;
        gspWaitForVBlank();
    }
}

void cleanupUI() {
    C2D_TextBufDelete(g_staticBuf);
    C2D_Fini();
}