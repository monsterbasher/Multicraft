#include <Aurora/System/FileManager.h>

namespace Aurora
{
	namespace System
	{
		FileManager::FileManager()
		{
			_fileManagerType = Normal;
			_mainDirPath = "";
		}

		FileManager::FileManager(FileManagerType fileManagerType)
		{
			_fileManagerType = fileManagerType;
			_mainDirPath = "";
		}

		FileManager::FileManager(FileManagerType fileManagerType,std::string pathName)
		{
			_fileManagerType = fileManagerType;
			_mainDirPath = pathName;
		}

		void FileManager::SetMainDirPath(std::string pathName)
		{
			_mainDirPath = pathName;
		}

		void FileManager::SetMainFile(std::string fileName)
		{
			_mainFileName = fileName;
		}

		bool FileManager::FileExists(std::string fileName)
		{
			return false;
		}

		File* FileManager::GetFile(std::string fileName)
		{
			return 0;
		}

		File* FileManager::_getDiskFile(std::string fileName)
		{
			return 0;
		}
		File* FileManager::_getZipFile(std::string fileName)
		{
			return 0;
		}
		File* FileManager::_getPackFile(std::string fileName)
		{
			return 0;
		}
	}
}

