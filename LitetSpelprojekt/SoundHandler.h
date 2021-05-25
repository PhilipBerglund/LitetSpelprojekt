#pragma once
#include "Xaudio2.h"
#include "ComPtr.h"
#include "AudioChunks.h"
#include <string>

class SoundHandler
{
private:
	static ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER audioBuffer = { 0 };
	IXAudio2SourceVoice* sourceVoice;

	//UnorderedMap f�r att spara ljudfiler
	//Enum f�r "tag" - f�rsta som None(default)

public:
	void Initialize();
	void AddSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails);
	void StartAudioEngine();
	void StopAudioEngine();
	void AddAudio(std::wstring fileName);
	void StartAudio();

	void DestroyAudio();
};

