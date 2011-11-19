#ifndef Image_H
#define Image_H

#include <string>

namespace Aurora
{
	namespace Graphics
	{
		class Image
		{
		public:

			int _width;
			int _height;
			int _textureWidth;
			int _textureHeight;

			unsigned int _id;
			unsigned char *_pixels;

			std::string _fileName;

			Image();
			~Image();

			bool loadImageFromFile(std::string fileName);
			bool loadImageFromMemory(std::string newName,void *data,std::size_t size);
			unsigned int getColour(int x,int y);
		};
	}
}

#endif