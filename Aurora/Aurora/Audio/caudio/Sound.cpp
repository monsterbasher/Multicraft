#include <Aurora/Audio/Sound.h>
#include <Aurora/Audio/caudio/cAudioManager.h>

#include <cAudio.h>

namespace Aurora
{
	namespace Audio
	{
		cAudio::IAudioSource* _sound;

		Sound::Sound(std::string name,std::string filePath,bool streaming)
		{
			_name = name;
			_filePath = filePath;
			_stream = streaming;

			_looped = false;
			_paused = false;

			_sound = ((cAudioManager*)AudioManager::Instance())->CreateSoudSource(name,filePath,streaming);
		}

		bool Sound::IsPlaying()
		{
			if (_sound)
			{
				return _sound->isPlaying();
			}

			return false;
		}

		void Sound::SetLoop(bool state)
		{
			_looped = state;
		}

		void Sound::Play()
		{
			if (_sound)
			{
				_sound->play2d(_looped);
			}
		}

		void Sound::Stop()
		{
			if (_sound)
			{
				_sound->stop();
			}
		}

		void Sound::Pause()
		{
			if (_sound)
			{
				_sound->pause();
			}
		}
	}
}

