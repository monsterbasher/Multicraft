#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace Graphics
	{
		TextureManager TextureManager::m_TextureManager;

		TextureManager* TextureManager::Instance()
		{
			return &m_TextureManager;
		}

		Image* TextureManager::loadImageFromFile(std::string fileName)
		{
			if(images.find(fileName) == images.end())
			{
				Image* image = new Image();
				if(image->loadImageFromFile(fileName))
				{
					//generate texture
					RenderManager::Instance()->_createTexture(image);

					//add to collection
					images.insert(std::pair<std::string,Image*>(fileName,image));

					return image;
				}else
				{
					//error
					return 0;
				}
			}

			return images[fileName];
		}

		Image* TextureManager::loadImageFromMemory(std::string newName,void *data,std::size_t size)
		{
			if(images.find(newName) == images.end())
			{
				Image* image = new Image();
				if(image->loadImageFromMemory(newName,data,size))
				{
					//generate texture
					RenderManager::Instance()->_createTexture(image);

					//add to collection
					images.insert(std::pair<std::string,Image*>(newName,image));

					return image;
				}else
				{
					//error
					return 0;
				}
			}

			return images[newName];
		}

		Image* TextureManager::getImage(std::string fileName)
		{
			if(images.find(fileName) == images.end())
			{
				return 0;
			}

			return images[fileName];
		}

		bool TextureManager::removeImage(std::string fileName)
		{
			std::map<std::string,Image*>::iterator it = images.find(fileName);

			if(it == images.end())
			{
				return false;
			}

			delete images[fileName];
			images.erase(it);
			return true;
		}
	}
}
