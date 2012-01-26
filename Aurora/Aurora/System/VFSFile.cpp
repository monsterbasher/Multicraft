#include <Aurora/System/VFSFile.h>

#include <stdio.h>

namespace Aurora
{
	namespace System
	{

		VFSFile::VFSFile(void* data, int length)
		{
			this->mData = data;
			this->mLength = length;
			offset = 0;
		}

		VFSFile::~VFSFile()
		{
			delete[] mData;
		}

		void VFSFile::SaveToDisk(const char* filename)
		{
			FILE* file = fopen(filename,"wb");

			fwrite(mData,1,mLength,file);

			fclose(file);
		}

	}
}

