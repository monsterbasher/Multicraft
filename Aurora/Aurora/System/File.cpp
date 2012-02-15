#include <Aurora/System/File.h>

namespace Aurora
{
	namespace System
	{
		File::File()
		{
			_fileName = "";
		}

		File::File(std::string fileName,std::string loadFileName)
		{
			_fileName = fileName;
			_loadFileName = loadFileName;
		}
	}
}
