#ifndef ZIPFILE_H 
#define ZIPFILE_H 

#include <unzip.h>
#include <string>

namespace Aurora
{
	namespace System
	{
		class ZipFile
		{
		private:

			unzFile _zipFile;

			std::string _fileName;

			bool _loaded;

		private:

			void _ListFilesInsideZip();

		public:

			ZipFile(std::string filename);

			bool OpenZip();
			void CloseZip();

			unsigned char* GetFileData(std::string fileInZip,int &dataSize);

			bool OpenFile(std::string fileInZip);
			void ReadFromFile(void *data,unsigned int size,int count);
			void CloseFile();

			bool IsLoaded() {return _loaded;}
		};
	}
}


#endif
