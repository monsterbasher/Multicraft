#include <Aurora/Audio/caudio/cAudioManager.h>

namespace Aurora
{
	namespace Audio
	{
		cAudioManager::cAudioManager()
		{
			manager = 0;
		}

		bool cAudioManager::Init()
		{
			manager = cAudio::createAudioManager(true);

			if (manager)
			{
				return true;
			}

			return false;
		}

		bool cAudioManager::SupportedFormat(AudioFormat type)
		{
			switch(type)
			{
				case MP3:
					return false;
				break;

				case WAV:
					return true;
				break;

				case OGG:
					return true;
				break;
			}

			return false;
		}

		cAudio::IAudioSource* cAudioManager::CreateSoudSource(std::string name,std::string filePath,bool srteaming)
		{
			cAudio::IAudioSource* _sound = manager->create(name.c_str(),filePath.c_str(),srteaming);
			return _sound;
		}
	}
}


