#pragma once
#include "Xaudio2.h"
#include "ComPtr.h"
#include "AudioChunks.h"

class SoundHandler
{
private:
	static ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	//UnorderedMap för att spara ljudfiler
	//Enum för "tag" - första som None(default)

public:

	bool Initialize();
	void addSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails);
	void StartAudioEngine();
	void StopAudioEngine();

	void DestroyAudio();

};

