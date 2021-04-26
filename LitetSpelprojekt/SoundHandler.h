#pragma once
#include "Xaudio2.h"
#include "ComPtr.h"

class SoundHandler
{
private:
	static ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;


public:

	bool Initialize();
	void addSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails);
	void StartAudio();
	void StopAudio();

	void DestroyAudio();

};

