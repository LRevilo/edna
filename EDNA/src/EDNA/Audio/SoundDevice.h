#pragma once
#include "AL/al.h"
#include "AL/alc.h"

 
namespace EDNA {

	 class SoundDevice
	 {
	 public:
		 static SoundDevice* Get();
		 SoundDevice();

		 ~SoundDevice();

		 static void Init();
		 uint16_t* s_AudioScratchBuffer;
		 uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

		 bool LoadSound(const char* path);

		  
	 private:
		 ALCdevice* p_ALCDevice;
		 ALCcontext* p_ALCContext;

	 };
}