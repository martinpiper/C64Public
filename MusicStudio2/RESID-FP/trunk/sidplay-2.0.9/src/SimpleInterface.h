#ifndef _SIMPLEINTERFACE_H_
#define _SIMPLEINTERFACE_H_

extern "C" void *AllocatePlayer(const char *flags,const char *filename);

extern "C" int PlayerGetMaxSong(void *handle);

extern "C" int PlayerGetCurrentSong(void *handle);

extern "C" int PlayerSetCurrentSong(void *handle , int song);

extern "C" bool PlayerPlay(void *handle);

extern "C" unsigned char PlayerGetMemoryByte(void *handle,const unsigned short address);

extern "C" void PlayerSetMemoryByte(void *handle,const unsigned short address,const unsigned char data);

extern "C" void PlayerFree(void *handle);

extern "C" void PlayerMute(void *handle,const int voice,const bool enable);

extern "C" float PlayerGetTime(void *handle);

class MemoryHook
{
public:
	MemoryHook() {}
	virtual ~MemoryHook() {}

	static MemoryHook *mMemoryHook;

	virtual void SIDRegisterWrite(const double clock,const unsigned char reg,const unsigned char data) = 0;
};

#endif
