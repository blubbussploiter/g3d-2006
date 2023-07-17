#include "sounds.h"

void RBX::Sound::stop()
{
	channel->stop();
}

void RBX::Sound::play()
{
	if(!sound)
		SoundService::singleton()->mpSystem->createSound(soundPath.c_str(), 0, 0, &sound);

	sound->setMode(FMOD_LOOP_OFF);
	SoundService::singleton()->mpSystem->playSound(sound, 0, 0, &channel);
}