//#include "Speaker.h"
//
//
//
//Speaker::Speaker()
//{
//	SDL_Init(SDL_INIT_AUDIO);
//
//	SDL_LoadWAV("gunshot.wav", &wavSpec, &wavBuffer, &wavLength);
//
//	wavSpec.callback = this->audioCallBack;
//	wavSpec.userdata = nullptr;
//
//	audioPos = wavBuffer;
//	audioLen = wavLength;
//
//	SDL_OpenAudio(&wavSpec, nullptr);
//}	
//
//
//Speaker::~Speaker()
//{
//	SDL_CloseAudio();
//	SDL_FreeWAV(wavBuffer);
//}
//
//
//void Speaker::playGunshot()
//{
//	SDL_PauseAudio(0);
//	while (audioLen > 0)
//	{
//		SDL_Delay(100);
//	}
//}
//
//
//void Speaker::audioCallBack(void* userData, Uint8* stream, int len)
//{
//	if (audioLen == 0)
//		return;
//
//	len = (len > audioLen ? audioLen : len);
//
//	SDL_MixAudio(stream, audioPos, len, SDL_MIX_MAXVOLUME);
//
//	audioPos += len;
//	audioLen -= len;
//}


