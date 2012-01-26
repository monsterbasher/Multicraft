#include <Aurora/System/VFSPack.h>
#include <Aurora/Utils/md5.h>
#include <Aurora/Utils/Crypto.h>

#include <zlib.h>

namespace Aurora
{
	namespace System
	{

		VFSPack::VFSPack(std::string fileName)
		{
			archiveFilename = fileName;
			_encryptionKey = NULL;
		}

		VFSPack::~VFSPack()
		{

		}

		void VFSPack::CreateNewPack(std::string filename)
		{
			archiveFilename = filename;
		}

		void VFSPack::AddFile(std::string filePath,bool compressed,bool encryption)
		{
			//add file info only if file really exists
			FILE *file = fopen(filePath.c_str(),"rb");
			if (file == NULL) return;

			long fileSize;

			//get file size
			fseek (file , 0 , SEEK_END);
			fileSize = ftell (file);
			fclose(file);

			//add file info
			PackFileInfo fileInfo;
			fileInfo.hash = hashString((unsigned char *)filePath.c_str());
			fileInfo.filename = filePath;

			fileInfo.compressed = compressed;
			fileInfo.encrypted = encryption;

			fileInfo.fileInPackPosition = 0;
			fileInfo.originalSize = fileSize;
			fileInfo.compressedSize = 0;
			fileInfo.encryptedSize = 0;			

			//add to collection
			_packedFiles.insert(std::pair<long,PackFileInfo>(fileInfo.hash,fileInfo));
		}


		bool VFSPack::SavePack()
		{
			//create file and write header
			FILE* file = fopen(archiveFilename.c_str(),"wb");

			int fileInPackPosition = 0;
			int fileInPackHeaderSize = (sizeof(int) * 4) + (sizeof(bool) * 2) + sizeof(long);
			int fileInPackFinalSize = 0;

			//write pack version
			int packVersion = 1;
			fwrite(&packVersion,sizeof(int),1,file);

			//write some options flags
			int flags = 0;
			fwrite(&flags,sizeof(int),1,file);

			//write files count
			int filesCount = _packedFiles.size();
			fwrite(&filesCount,sizeof(int),1,file);

			//pack header size
			fileInPackPosition = sizeof(int) * 3;

			std::map<long,PackFileInfo>::iterator it;

			//1. iterate on all files info and load files into the memory with compression,encryption
			for ( it= _packedFiles.begin() ; it != _packedFiles.end(); it++ )
			{
				//open each file listed and add to pack file
				FILE *fileToPack = fopen(it->second.filename.c_str(),"rb");

				char* buffer = (char*) malloc (sizeof(char)*it->second.originalSize);
				fread (buffer,1,it->second.originalSize,fileToPack);

				fclose(fileToPack);

				//if not compressed and not encrypted
				if (!it->second.compressed && !it->second.encrypted)
				{
					it->second.buffer = (char*)malloc(sizeof(char)*it->second.originalSize);
					memset(it->second.buffer,0,it->second.originalSize);
					memcpy(it->second.buffer,buffer,it->second.originalSize);
					
					//clean buffer
					free(buffer);
				}
				else if (it->second.compressed && !it->second.encrypted)
				{
					//compress only
					int compressSize = 0;

					CompressString(buffer,it->second.originalSize,&it->second.buffer,&compressSize);

					it->second.compressedSize = compressSize;
					
					//clean buffer
					free(buffer);
				}
				else if (!it->second.compressed && it->second.encrypted)
				{
					//encrypt only
					int encryptSize = 0;

					Utils::aesEncrypt(_encryptionKey,buffer,it->second.originalSize,&it->second.buffer,&encryptSize);

					it->second.encryptedSize = encryptSize;

					//clean buffer
					free(buffer);
				}
				else if (it->second.compressed && it->second.encrypted)
				{
					//compress and encrypt
					int compressSize = 0;
					int encryptSize = 0;
					char* tempBuffer;

					CompressString(buffer,it->second.originalSize,&tempBuffer,&compressSize);
					Utils::aesEncrypt(_encryptionKey,tempBuffer,compressSize,&it->second.buffer,&encryptSize);

					it->second.compressedSize = compressSize;
					it->second.encryptedSize = encryptSize;

					//clean buffers
					free(buffer);
					free(tempBuffer);
				}
			}

			fileInPackPosition += (fileInPackHeaderSize * _packedFiles.size());

			//2. save all file info headers
			for ( it= _packedFiles.begin() ; it != _packedFiles.end(); it++ )
			{
				fwrite(&it->second.hash,sizeof(long),1,file);

				fwrite(&it->second.encrypted,sizeof(bool),1,file);
				fwrite(&it->second.compressed,sizeof(bool),1,file);

				fwrite(&fileInPackPosition,sizeof(int),1,file);
				fwrite(&it->second.originalSize,sizeof(int),1,file);
				fwrite(&it->second.compressedSize,sizeof(int),1,file);
				fwrite(&it->second.encryptedSize,sizeof(int),1,file);

				//correct position in file
				if (!it->second.compressed && !it->second.encrypted)
				{
					fileInPackPosition += it->second.originalSize;
				}
				else if (it->second.compressed && !it->second.encrypted)
				{
					fileInPackPosition += it->second.compressedSize;
				}
				else if (!it->second.compressed && it->second.encrypted)
				{
					fileInPackPosition += it->second.encryptedSize;
				}
				else if (it->second.compressed && it->second.encrypted)
				{
					fileInPackPosition += it->second.encryptedSize;
				}
			}

			//3. save all files data
			for ( it= _packedFiles.begin() ; it != _packedFiles.end(); it++ )
			{
				int bufferSize = 0;

				if (!it->second.compressed && !it->second.encrypted)
				{
					bufferSize = it->second.originalSize;
				}
				else if (it->second.compressed && !it->second.encrypted)
				{
					bufferSize = it->second.compressedSize;
				}
				else if (!it->second.compressed && it->second.encrypted)
				{
					bufferSize = it->second.encryptedSize;
				}
				else if (it->second.compressed && it->second.encrypted)
				{
					bufferSize = it->second.encryptedSize;
				}

				fwrite(it->second.buffer,sizeof(char),bufferSize,file);
			}


			fclose (file);
			return false;
		}

		void VFSPack::SetEncryptKey(std::string key)
		{
			if (_encryptionKey != NULL)
			{
				free(_encryptionKey);
			}

			int test = key.length();
			char *test2 = (char *)key.c_str();

			_encryptionKey = new unsigned char[test];//(unsigned char *)malloc(test);

			memset(_encryptionKey,0,test);
			memcpy(_encryptionKey,test2, test); 

		}

		unsigned long VFSPack::hashString(unsigned char *str)
		{
			unsigned long hash = 0;
			int c;

			while (c = *str++)
				hash = c + (hash << 6) + (hash << 16) - hash;

			return hash;
		}

		VFSFile VFSPack::GetData(std::string filename)
		{
			unsigned char md5[16];
			CalcMD5(filename, md5);

			FILE * File = fopen(archiveFilename .c_str(), "rb");

			/*if (!File)
			{
			  return NULL;
			}*/

			int Version;
			int Flags;
			int FilesCount;

			fread(&Version, sizeof(int), 1, File);
			fread(&Flags, sizeof(int), 1, File);
			fread(&FilesCount, sizeof(int), 1, File);

			//if (Version != 1) throw "Unsupported version.";

			int Pos = -1, OLength, Length;

			for (int i = 0; i < FilesCount; i++)
			{
				unsigned char fmd5[16];

				fread(fmd5, 1, 16, File);

				bool ok = true;
				for (int j = 0; j < 16; j++)
				{
					  if (fmd5[j] != md5[j])
					  {
							ok = false;
							break;
					  }
				}

				fread(&Pos, sizeof(int), 1, File);
				fread(&OLength, sizeof(int), 1, File);
				fread(&Length, sizeof(int), 1, File);

				if (ok)
				{
					  break;
				}
				else
				{
					  Pos = -1;
				}
			}

			//if (Pos==-1) throw "File not found.";

			fseek(File, Pos, 0);

			//read file
			char * data = new char[Length];
			fread(data, Length, 1, File);
			fclose(File);

			char* decompressed;
			int result;

			result = DecompressString(data, Length, &decompressed, &OLength,OLength);

			fclose(File);

			return VFSFile(decompressed, OLength);
		}

		void VFSPack::vFread(void *ptr, size_t size, size_t n,VFSFile &vFile)
		{
			int readSize = 0, realSize = size * n;

			readSize = CVFMin(realSize, vFile.GetLength() - vFile.offset);
			if (readSize > 0)
			{
				memcpy(ptr, (char*)vFile.GetData() + vFile.offset, readSize);
				vFile.offset += readSize;
			}
		}

		void VFSPack::CalcMD5(std::string text, unsigned char *out)
		{
			md5_state_t state;
			md5_init(&state);
			md5_append(&state, (const md5_byte_t*)text.c_str(), (int)strlen(text.c_str()));
			md5_finish(&state, out);
		}

		int VFSPack::CompressString(char* src,int srcLen,char** destination, int* destLength)
		{
			//Define the source, destination, source length, and destination length
			int destLen=compressBound(srcLen);
			char *dest=new char[destLen];
			//Decompress the string in src and place it in dest
			int result=compress((unsigned char *)dest,(uLongf*)&destLen,(const unsigned char *)src,srcLen);
			//Return the results of the compression
			*destination = dest;
			*destLength = destLen;
			return(result);
		}

		int VFSPack::DecompressString(char* src, int srcLen, char** destination, int* destLen,int dupa)
		{
			//Define the source, destination, source length, and destination length
			char *dest=new char[dupa];
			//Decompress the string in src and place it in dest
			int result=uncompress((unsigned char *)dest,(uLongf*)destLen,(const unsigned char *)src,srcLen);
			//Return the results of the decompression
			*destination = dest;
			return(result);
		}

	}
}

