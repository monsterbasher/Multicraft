#ifndef cAudioManager_H
#define cAudioManager_H

#include <Aurora/Audio/AudioManager.h>

#include <cAudio.h>

namespace Aurora
{
	namespace Audio
	{
		class cAudioManager : public AudioManager
		{
		private:

			cAudio::IAudioManager* manager;

		public:

			cAudioManager();

			bool Init();
			bool SupportedFormat(AudioFormat type);

		public:

			cAudio::IAudioSource* CreateSoudSource(std::string name,std::string filePath,bool srteaming);
		};
	}
}

#endif
