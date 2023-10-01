#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Tamagotchi.h"
#include "Graphic.h"

const unsigned char SCREEN_WIDTH = 32;
const unsigned char SCREEN_HEIGHT = 16;

bool screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
Graphic* screenGraphics[8];

int gameLoop()
{
	const std::vector<unsigned char> pixels = { 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2 };
	const unsigned char width = 9;
	const unsigned char height = 9;
	Graphic graphic = Graphic(pixels, width, height);
	char xPos = 0;
	char yPos = 0;
	graphic.screenPos = Position(xPos, yPos);
	graphic.transformation = Tranformation::Default;

	//addGraphic(&graphic);
	loadGraphics(0);

	//std::cout << "Size: " + std::to_string(pixels.size()) + " Expected size: " + std::to_string(width * height);

	while (true)
	{
		//render();
		std::string input;
		std::cin >> input;
	}

	return 0;
}

void render()
{
	clearScreenBuffer();
	updateScreenBuffer();
	drawToScreen();
}

int loadGraphic(Graphic* graphic, unsigned char id)
{
	std::ifstream inputFile("D:\\repos\\learn\\learn\\Graphics\\Graphics.bin", std::ios::binary);

	if (!inputFile)
	{
		std::cerr << "Failed to open file!" << std::endl;
		return 1;
	}

	std::vector<unsigned char> buffer;
	unsigned char width, height;
	
	char byte, byte2;
	unsigned short int blockLength = 0;
	unsigned char blockID = 0;

	while (inputFile.read(&byte, 1))
	{
		if (blockLength == 0)
		{
			//start of the next block
			blockID++;
			inputFile.read(&byte2, 1);
			width = byte;
			height = byte2;
			blockLength = width * height;
		}

		blockLength--;
	}

	delete& byte;
	delete& byte2;
	delete& blockLength;

	std::cout << std::endl;

	inputFile.close();

	return 0;
}

void updateScreenBuffer()
{
	for (int i = 0; i < sizeof(screenGraphics) / sizeof(screenGraphics[0]); i++)
	{
		Graphic* graphic = screenGraphics[i];

		//std::cout << "Placing graphic at screen pos x: " + std::to_string(graphic->screenPos.x) + " y: " + std::to_string(graphic->screenPos.y) << std::endl;

		for (int j = 0; j < graphic->WIDTH * graphic->HEIGHT; j++)
		{
			unsigned char pixel = graphic->PIXELS[j];

			if (pixel == 0)
			{
				setScreenPixelByGraphicBufferIndex(graphic, j, false);
			}
			else if (pixel == 1)
			{
				setScreenPixelByGraphicBufferIndex(graphic, j, true);
			}
		}
	}
}

void setScreenPixelByGraphicBufferIndex(Graphic* graphic, unsigned char index, bool value)
{
	unsigned char x = (index % graphic->WIDTH);
	unsigned char y = (index / graphic->WIDTH);

	switch (graphic->transformation)
	{
		case Tranformation::HorizontalMirror:
			x = graphic->WIDTH - 1 - x;
			y = graphic->HEIGHT - 1 - y;
			break;
		case Tranformation::VerticalMirror:
			break;
		default:
			y = graphic->HEIGHT - 1 - y;
			break;
	}

	x += graphic->screenPos.x;
	y += graphic->screenPos.y;

	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	{
		//Only set if we're in screen bounds
		screenBuffer[y * SCREEN_WIDTH + x] = value;
	}
}

void clearScreenBuffer()
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		screenBuffer[i] = false;
	}
}

void drawToScreen()
{
	screenBuffer[0] = true;

	std::system("cls");

	char printLine[SCREEN_WIDTH + 1];
	printLine[SCREEN_WIDTH] = '\0'; //Null terminator

	int screenIndex = 0;
	for (int y = SCREEN_HEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			printLine[x] = (screenBuffer[y * SCREEN_WIDTH + x]) ? (char)254u : '#';
			screenIndex++;
		}
		std::cout << printLine << std::endl;
	}
}