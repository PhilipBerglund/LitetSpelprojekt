#pragma once
#include "Xaudio2.h"
#include "ComPtr.h"
#include "AudioChunks.h"
#include <string>

class SoundHandler
{
private:
	static ComPtr<IXAudio2> pXAudio2;
	static IXAudio2MasteringVoice* masterVoice;

	static WAVEFORMATEXTENSIBLE wfx;
	static XAUDIO2_BUFFER audioBuffer;
	static IXAudio2SourceVoice* sourceVoice;
	static float volume;
	static bool muted;
	//UnorderedMap för att spara ljudfiler
	//Enum för "tag" - första som None(default)

public:
	static void Initialize();
	static void AddSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails);
	static void StartAudioEngine();
	static void StopAudioEngine();
	static void AddAudio(std::wstring fileName);
	static void StartAudio();
	static void SetVolume(float volume);
	static float GetVolume();
	static void DestroyAudio();
	static bool IsMuted() { return muted; }
	static void Mute() { muted = true; }
	static void Unmute() { muted = false; }
};

