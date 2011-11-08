#ifndef TextureManager_H
#define TextureManager_H

#include <Aurora/Graphics/Image.h>

#include <string>
#include <map>

namespace Aurora
{
	namespace Graphics
	{
		class TextureManager
		{
		private:
			
			static TextureManager m_TextureManager;
			std::map<std::string,Image*> images;

		private:

			TextureManager()
			{

			}

		public:

			static TextureManager* Instance();

			Image* loadImageFromFile(std::string fileName);
			Image* loadImageFromMemory(std::string newName,void *data,std::size_t size);

			Image* getImage(std::string fileName);
			bool removeImage(std::string fileName);
		};
	}
}

#endif
