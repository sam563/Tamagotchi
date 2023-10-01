#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "lodepng.h"

std::vector<uint8_t>* pngToTamagotchiGraphic(std::string& pngPath);
int pngDirectoryToTamagotchiFileWrite(std::string& pngDirectory, std::string& outputPath);
std::vector<std::filesystem::path>* getPNGFilesInDirectory(std::string& directory);
void getPixelNoAlphaMode(bool& p, unsigned char r);
void getPixelAlphaMode(bool& p, bool& al, unsigned char r, unsigned char a);

struct PackedBooleans
{
	bool bit1 : 1;
	bool bit2 : 1;
	bool bit3 : 1;
	bool bit4 : 1;
	bool bit5 : 1;
	bool bit6 : 1;
	bool bit7 : 1;
	bool bit8 : 1;
};