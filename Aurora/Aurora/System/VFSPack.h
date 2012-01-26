#ifndef VFSPACK_H_
#define VFSPACK_H_

#include <Aurora/System/VFSFile.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <map>

#define CVFMin(x, y)				(((x)<(y))?(x):(y))

namespace Aurora
{
	namespace System
	{
		class PackFileInfo
		{
		public:

			long hash;
			std::string filename;

			bool encrypted;
			bool compressed;

			char* buffer;

			int fileInPackPosition;

			int originalSize;
			int compressedSize;
			int encryptedSize;
		};

		class VFSPack
		{
		public:
			VFSPack(){ _encryptionKey = NULL; }
			VFSPack(std::string fileName);
			virtual ~VFSPack();

			VFSFile GetData(std::string filename);

			//reading elements from virtual file
			static void vFread(void *ptr, size_t size, size_t n,VFSFile &vFile);

			void CreateNewPack(std::string filename);
			void AddFile(std::string filePath,bool compressed,bool encryption);
			bool SavePack();

			void SetEncryptKey(std::string key);
		private:
			//all files in pack

			std::map<long,PackFileInfo> _packedFiles;

			///
			unsigned char *_encryptionKey;
			std::string archiveFilename;

			void CalcMD5(std::string text, unsigned char *out);
			unsigned long hashString(unsigned char *str);

			//compression and decompression
			int CompressString(char* src,int srcLen, char** destination, int* destLength);
			int DecompressString(char* src, int srcLen, char** destination, int* destLen,int dupa);
		};

	}
}

#endif

