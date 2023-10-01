#include "TamagotchiGraphicWriter.h"

namespace fs = std::filesystem;

std::vector<uint8_t>* pngToTamagotchiGraphic(std::string& pngPath)
{
	std::vector<unsigned char> image;
	unsigned int width, height;

	unsigned int error = lodepng::decode(image, width, height, pngPath);

	if (error)
	{
		std::cerr << "Error while loading PNG: " << lodepng_error_text(error) << std::endl;
		return nullptr;
	}
	
	std::vector<uint8_t>* buffer = new std::vector<uint8_t>();
	buffer->push_back(width);

	bool x1, al1, x2, al2, x3, al3, x4, al4, x5, x6, x7, x8;

	bool alphaMode = false;

	//Do a pass through all the pixel alphas to see if we're using the alpha channel
	for (int i = 3; i < image.size(); i += 4)
	{
		al1 = image[i];
		if (al1 == 0)
		{
			alphaMode = true;
			break;
		}
	}

	std::cout << "Alpha mode " << ((alphaMode) ? "ENABLED" : "DISABLED") << " for image " << pngPath << std::endl;

	if (alphaMode)
	{
		image.resize(std::ceil(image.size() / 16) * 16); //make sure the buffer is a multiple of 16 so we don't go out of bounds

		for (int i = 0; i < image.size(); i++)
		{
			getPixelAlphaMode(x1, al1, image[i], image[i += 3]);
			getPixelAlphaMode(x2, al2, image[i += 1], image[i += 3]);
			getPixelAlphaMode(x3, al3, image[i += 1], image[i += 3]);
			getPixelAlphaMode(x4, al4, image[i += 1], image[i += 3]);

			PackedBooleans map;
			map.bit1 = x1;
			map.bit2 = al1;
			map.bit3 = x2;
			map.bit4 = al2;
			map.bit5 = x3;
			map.bit6 = al3;
			map.bit7 = x4;
			map.bit8 = al4;

			buffer->push_back(*reinterpret_cast<uint8_t*>(&map));
		}

		//calculate where the position of the last bit is, if it is in use, add another byte
		//set the final bit to be the alpha mask mode
		if ((width * height * 2) % 8 == 0) //2 bits for each pixel, of or on and alpha off or on
		{
			buffer->push_back(*new uint8_t());
		}

		buffer->back() |= 1 << 7; //This should set the last bit to 1, meaning alpha is enabled
	}
	else
	{
		image.resize(std::ceil(image.size() / 32) * 32); //make sure the buffer is a multiple of 32 so we don't go out of bounds

		for (int i = 0; i < image.size(); i += 4)
		{
			getPixelNoAlphaMode(x1, image[i]);
			getPixelNoAlphaMode(x2, image[i += 4]);
			getPixelNoAlphaMode(x3, image[i += 4]);
			getPixelNoAlphaMode(x4, image[i += 4]);
			getPixelNoAlphaMode(x5, image[i += 4]);
			getPixelNoAlphaMode(x6, image[i += 4]);
			getPixelNoAlphaMode(x7, image[i += 4]);
			getPixelNoAlphaMode(x8, image[i += 4]);

			PackedBooleans map;
			map.bit1 = x1;
			map.bit2 = x2;
			map.bit3 = x3;
			map.bit4 = x4;
			map.bit5 = x5;
			map.bit6 = x6;
			map.bit7 = x7;
			map.bit8 = x8;

			buffer->push_back(*reinterpret_cast<uint8_t*>(&map));

			if ((width * height) % 8 == 0)
			{
				buffer->push_back(*new uint8_t());
			}

			buffer->back() &= 0b01111111; //This should set the last bit to 0, meaning alpha is disabled
		}
	}

	std::cout << "Wrote " << buffer->size() << " bytes to buffer!" << std::endl;

	return buffer;
}

void getPixelNoAlphaMode(bool& p, unsigned char r)
{
	p = (r > 0) ? 1 : 0;
}

void getPixelAlphaMode(bool& p, bool& al, unsigned char r, unsigned char a)
{
	if (a > 0)
	{
		if (r > 0)
		{
			//white pixel
			p = 1;
			al = 1;
		}
		else
		{
			//black pixel
			p = 0;
			al = 1;
		}
	}
	else
	{
		//cull pixel
		p = 0;
		al = 0;
	}
}

std::vector<fs::path>* getPNGFilesInDirectory(std::string& directory)
{
	if (!fs::exists(directory) || !fs::is_directory(directory))
	{
		std::cerr << "Invalid directory path: " << directory << std::endl;
		return nullptr;
	}

	std::vector<fs::path>* result = new std::vector<fs::path>;

	for (const auto& entry : fs::recursive_directory_iterator(directory))
	{
		if (fs::is_regular_file(entry))
		{
			//std::cout << "Found file in directory! " << entry.path() << std::endl;
			std::string extension = entry.path().extension().string();

			if (extension == ".png")
			{
				result->push_back(entry.path());
			}
		}
	}

	return result;
}

int pngDirectoryToTamagotchiFileWrite(std::string& pngDirectory, std::string& outputPath)
{
	std::vector<fs::path>* paths = getPNGFilesInDirectory(pngDirectory);

	std::vector<std::vector<uint8_t>*> imageBuffers(paths->size());

	std::cout << "Found " << paths->size() << " .png file(s) in directory " << pngDirectory << "!" << std::endl;

	for (size_t i = 0; i < paths->size(); i++)
	{
		std::string path = (*paths)[i].string();
		imageBuffers[i] = pngToTamagotchiGraphic(path);
	}

	delete paths;

	std::vector<uint8_t> buffer(imageBuffers.size() * 2); //2 bytes per pointer (unsigned short int)

	unsigned short int currentPos = imageBuffers.size() + 1;
	for (int i = 0; i < imageBuffers.size(); i++)
	{
		buffer[i] = (currentPos >> 8) & 0xFF; //First byte of currentPos
		buffer[i + 1] = currentPos & 0xFF; //Second byte of currentPos

		currentPos += imageBuffers[i]->size();
		//buffer.resize(currentPos);
		buffer.insert(buffer.end(), imageBuffers[i]->begin(), imageBuffers[i]->end());
	}

	//std::cout << "Writing output (" << sizeof(buffer) << ") bytes to file " << outputPath << std::endl;

	std::ofstream outputFile(outputPath, std::ios::out | std::ios::binary);

	if (!outputFile.is_open()) {
		std::cerr << "Failed to open the file for writing." << std::endl;
		return 1;
	}

	outputFile.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));

	if (!outputFile) {
		std::cerr << "Error occurred while writing to the file." << std::endl;
		return 1; // Return an error code
	}

	outputFile.close();

	if (outputFile.fail())
	{
		std::cerr << "Error occurred while writing to the file!" << std::endl;
		return 1;
	}

	std::cout << "COMPLETE: " << imageBuffers.size() << " images written to " << outputPath << " with size " << buffer.size() << " bytes!";

	return 0;
}