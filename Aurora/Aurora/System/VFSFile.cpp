#include <Aurora/System/VFSFile.h>

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

	}
}

