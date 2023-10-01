#include <string>
#include "TamagotchiGraphicWriter.h"

int main()
{
	std::string pngPath = "D:\\repos\\Tamagotchi\\Tamagotchi\\Graphics";
	std::string outputPath = "D:\\repos\\Tamagotchi\\Tamagotchi\\Graphics\\Graphics.bin";
	return pngDirectoryToTamagotchiFileWrite(pngPath, outputPath);
}