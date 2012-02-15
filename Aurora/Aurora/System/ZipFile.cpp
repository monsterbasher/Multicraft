#include <Aurora/System/ZipFile.h>

namespace Aurora
{
	namespace System
	{
		void ZipFile::CloseFile()
		{
			unzCloseCurrentFile(_zipFile);
		}

		ZipFile::ZipFile(std::string filename)
		{
			_fileName = filename;
			_loaded = false;
		}

		bool ZipFile::OpenZip()
		{
			_zipFile = unzOpen(_fileName.c_str());

			if (_zipFile != NULL)
			{
				//_ListFilesInsideZip();
				_loaded = true;
			}

			return _loaded;
		}

		void ZipFile::CloseZip()
		{
			unzClose(_zipFile);
		}

		bool ZipFile::OpenFile(std::string fileInZip)
		{
			int errorFlag = 0;
			char zipFileName[256];
			unz_file_info fileInfo;

			//locate file in zip 
			if (unzLocateFile(_zipFile,fileInZip.c_str(),0) != UNZ_OK)
			{
				return false;
			}

			//get info about file
			errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

			if (errorFlag != UNZ_OK)
				return false;

			errorFlag = unzOpenCurrentFile(_zipFile);
			if (errorFlag != UNZ_OK)
				return false;

			return true;
		}

		void ZipFile::ReadFromFile(void *data,unsigned int size,int count)
		{
			unzReadCurrentFile(_zipFile,data,size * count);
		}


		void ZipFile::_ListFilesInsideZip()
		{
			unz_global_info globalInfo;
			int errorFlag = 0;

			//get info from zip file
			errorFlag = unzGetGlobalInfo(_zipFile,&globalInfo);

			if (errorFlag != UNZ_OK)
			{
				//error
				return;
			}

			//go thou all files
			for (unsigned int i = 0;i < globalInfo.number_entry;i++)
			{
				char zipFileName[256];
				unz_file_info fileInfo;

				//get file info
				errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

				if (errorFlag != UNZ_OK)
				{
					//printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
					break;
				}


				//go to next file
				if ((i+1) < globalInfo.number_entry)
				{
					errorFlag = unzGoToNextFile(_zipFile);
					if (errorFlag != UNZ_OK)
					{
						//printf("error %d with zipfile in unzGoToNextFile\n",err);
						break;
					}
				}
			}
		}

		unsigned char* ZipFile::GetFileData(std::string fileInZip,int &dataSize)
		{
			int errorFlag = 0;
			char zipFileName[256];
			unz_file_info fileInfo;
			unsigned char* data;

			//locate file in zip 
			if (unzLocateFile(_zipFile,fileInZip.c_str(),0) != UNZ_OK)
			{
				return NULL;
			}

			//get info about file
			errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

			if (errorFlag != UNZ_OK)
				return NULL;

			//create data buffer
			dataSize = fileInfo.uncompressed_size;
			data = new unsigned char[fileInfo.uncompressed_size];

			//open file
			errorFlag = unzOpenCurrentFile(_zipFile);
			if (errorFlag != UNZ_OK)
				return NULL;

			//read file content
			unzReadCurrentFile(_zipFile,data,fileInfo.uncompressed_size);

			unzCloseCurrentFile(_zipFile);

			return data;
		}

	}
}


