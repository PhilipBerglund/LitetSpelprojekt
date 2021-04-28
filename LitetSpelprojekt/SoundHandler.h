#pragma once
#include "Xaudio2.h"
#include "ComPtr.h"
#include "AudioChunks.h"

class SoundHandler
{
private:
	static ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	//UnorderedMap f�r att spara ljudfiler
	//Enum f�r "tag" - f�rsta som None(default)

public:

	bool Initialize();
	void addSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails);
	void StartAudioEngine();
	void StopAudioEngine();

	void DestroyAudio();

};

