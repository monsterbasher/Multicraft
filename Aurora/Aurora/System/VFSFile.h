#ifndef VFSFILE_H_
#define VFSFILE_H_

namespace Aurora
{
	namespace System
	{
		class VFSFile
		{
		public:
			VFSFile(void* data, int length);
			virtual ~VFSFile();

			void* GetData() {return mData;}
			int GetLength() {return mLength;}
			void Rewind() {offset = 0; }

			int offset;

		private:
			void* mData;
			int mLength;
		};
	}
}

#endif

