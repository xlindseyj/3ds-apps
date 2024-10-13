#include <citro2d.h>
#include <3ds.h>
#include <string.h>
#include "ui.h"

// Screen dimensions
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

void initUI() {
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    C2D_SceneBegin(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT));
}

void displayMessage(const char* message) {
    // Clear screen before displaying message
    C2D_TargetClear(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT), C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));

    // Create a buffer for rendering text
    C2D_TextBuf g_staticBuf = C2D_TextBufNew(4096); // Buffer for static text
    C2D_Text g_staticText;

    // Parse and optimize text
    C2D_TextParse(&g_staticText, g_staticBuf, message);
    C2D_TextOptimize(&g_staticText);

    // Render text in the middle of the screen
    C2D_SceneBegin(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT));
    C2D_DrawText(&g_staticText, C2D_WithColor, (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 10) / 2, 0.5f, 0.5f, 0.5f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));

    // Flush and swap buffers
    C2D_Flush();
    gfxFlushBuffers();
    gfxSwapBuffers();
}

void cleanupUI() {
    C2D_Fini();
    gfxExit();
}
