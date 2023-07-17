#ifndef SOUND_H
#define SOUND_H

#include "fmod.hpp"
#include "instance.h"

#include <vector>
#include <string>

namespace RBX
{
	class Sound : RBX::Instance
	{
	private:
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		bool b3d;
	public:
		bool isLooping;
		bool stream;

		std::string soundPath;
		void play();
		void stop();
		bool isPlaying() { bool b; channel->isPlaying(&b); return b; }
		static RBX::Sound* fromFile(std::string file, bool isLooped = 0)
		{
			RBX::Sound* s = new RBX::Sound();
			s->soundPath = file;
			s->isLooping = isLooped;
			return s;
		}
		virtual ~Sound() { sound->release(); }
	};

	class SoundService
	{
	public:
		FMOD::System* mpSystem;

		void init();
		void update();

		static SoundService* singleton();
	};

}

#endif