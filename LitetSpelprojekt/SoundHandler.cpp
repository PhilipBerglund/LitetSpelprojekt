#include "SoundHandler.h"

ComPtr<IXAudio2> SoundHandler::pXAudio2;

void SoundHandler::Initialize()
{
    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
	{
		ERROR("FAILED TO INITIALIZE EX");
	}

    pXAudio2 = nullptr;
    hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr))
	{
		ERROR("FAILED TO CREATE XAUDIO2");
	}

    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&masterVoice)))
	{
		ERROR("FAILED TO CREATE MASTERING VOICE");
	}
}

void SoundHandler::AddSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails)
{
    masterVoice->GetVoiceDetails(&voiceDetails);
}

void SoundHandler::StartAudioEngine()
{
    pXAudio2->StartEngine();
}

void SoundHandler::StopAudioEngine()
{
    pXAudio2->StopEngine();
}

void SoundHandler::AddAudio(std::wstring fileName)
{
#ifdef _XBOX
	char* strFileName = fileName;
#else
	LPCWSTR strFileName = fileName.c_str();
#endif

	Initialize();

	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		ERROR("INVALID HANDLE VALUE");

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		ERROR("INVALID SET FILE POINTER");

	DWORD audioChunkSize;
	DWORD audioChunkPosition;
	FindChunk(hFile, fourccRIFF, audioChunkSize, audioChunkPosition);
	DWORD fileType;
	ReadChunkData(hFile, &fileType, sizeof(DWORD), audioChunkPosition);
	if (fileType != fourccWAVE)
		ERROR("This isn't WAVE file!\n");
	else ERROR("WAVE format!\n");

	FindChunk(hFile, fourccFMT, audioChunkSize, audioChunkPosition);
	ReadChunkData(hFile, &wfx, audioChunkSize, audioChunkPosition);

	FindChunk(hFile, fourccDATA, audioChunkSize, audioChunkPosition);
	BYTE* dataBuffer = new BYTE[audioChunkSize];
	ReadChunkData(hFile, dataBuffer, audioChunkSize, audioChunkPosition);

	audioBuffer.AudioBytes = audioChunkSize;
	audioBuffer.pAudioData = dataBuffer;
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

	HRESULT hr;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL,NULL,NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
}

void SoundHandler::StartAudio()
{
	HRESULT hr;
	if (FAILED(hr = sourceVoice->SubmitSourceBuffer(&audioBuffer, nullptr)))
	{
		ERROR("FAILED TO SUBMIT SOURCE BUFFER");
	}

	if (FAILED(hr = sourceVoice->Start(0)))
	{
		ERROR("FAILED TO START SOURCE VOICE");
	}
}

void SoundHandler::SetVolume(float volume)
{
	sourceVoice->SetVolume(volume);
}

void SoundHandler::DestroyAudio()
{

}
