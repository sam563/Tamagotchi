#pragma once

#include "Graphic.h"

int gameLoop();
void render();
void drawToScreen();
void updateScreenBuffer();
void setScreenPixelByGraphicBufferIndex(Graphic* graphic, unsigned char index, bool value);
void clearScreenBuffer();
void addGraphic(Graphic* graphic);
int loadGraphics(unsigned char id);