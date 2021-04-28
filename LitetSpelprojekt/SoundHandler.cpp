#include "SoundHandler.h"

ComPtr<IXAudio2> SoundHandler::pXAudio2;

bool SoundHandler::Initialize()
{
    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
        return hr;

    pXAudio2 = nullptr;
    hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr))
        return hr;

    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&masterVoice)))
        return hr;
}

void SoundHandler::addSourceVoiceDetails(XAUDIO2_VOICE_DETAILS voiceDetails)
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

void SoundHandler::DestroyAudio()
{

}
