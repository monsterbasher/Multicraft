#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <Aurora/System/File.h>

#include <string>

namespace Aurora
{
	namespace System
	{
		enum FileManagerType
		{
			Normal,
			Zip,
			Pack
		};

		class FileManager
		{
		private:

			FileManagerType _fileManagerType;

			std::string _mainDirPath;
			std::string _mainFileName;

		private:

			File *_getDiskFile(std::string fileName);
			File *_getZipFile(std::string fileName);
			File *_getPackFile(std::string fileName);

		public:

			FileManager();
			FileManager(FileManagerType fileManagerType);
			FileManager(FileManagerType fileManagerType,std::string pathName);

			void SetMainDirPath(std::string pathName);
			void SetMainFile(std::string fileName);

			bool FileExists(std::string fileName);
			File *GetFile(std::string fileName);
		};
	}
}

#endif
