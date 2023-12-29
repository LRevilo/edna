#include "ednapch.h"
#include "SoundDevice.h"
#include "sndfile.hh"

namespace EDNA {



SoundDevice* SoundDevice::Get()
{
	static SoundDevice* snd_device = new SoundDevice();
	return snd_device;
}



void SoundDevice::Init()
{
	// Init listener
	ALfloat listenerPos[] = { 0.0,0.0,0.0 };
	ALfloat listenerVel[] = { 0.0,0.0,0.0 };
	ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	EDNA_CORE_INFO("SoundDevice Init");
}
bool SoundDevice::LoadSound(const char* path)
{
	SndfileHandle soundFile = SndfileHandle(path);
	EDNA_CORE_TRACE("Channels: {0}", soundFile.channels() );
	EDNA_CORE_TRACE("Frames: {0}", soundFile.frames() );
	EDNA_CORE_TRACE("Samplerate: {0}", soundFile.samplerate());

	std::string format = "None";
	uint32_t formatSize = 0;
	switch (soundFile.format())
	{
	case SF_FORMAT_WAV | SF_FORMAT_PCM_32:
		format = "WAV32";
		formatSize = 4;
		break;
	case SF_FORMAT_WAV | SF_FORMAT_PCM_24:
		format = "WAV24";
		formatSize = 3;
		break;
	case SF_FORMAT_WAV | SF_FORMAT_PCM_16:
		format = "WAV16";
		formatSize = 2;
		break;
	case SF_FORMAT_WAV | SF_FORMAT_PCM_S8:
		format = "WAV8";
		formatSize = 1;
		break;
	default:
		EDNA_CORE_TRACE("Invalid format");
		return false;
	}
	EDNA_CORE_TRACE("Format: {0}", format);


	
	uint32_t bufferSize = formatSize * soundFile.channels() * soundFile.frames();

	if (s_AudioScratchBufferSize < bufferSize)
	{
		EDNA_CORE_TRACE("Resizing scratch buffer...");
		s_AudioScratchBufferSize = bufferSize;
		delete[] s_AudioScratchBuffer;
		s_AudioScratchBuffer = new uint16_t[s_AudioScratchBufferSize];
	}
	auto readResult = soundFile.read((short*)s_AudioScratchBuffer, soundFile.channels() * soundFile.frames());

	EDNA_CORE_TRACE("Read: {0}", readResult);

	

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, AL_FORMAT_STEREO16, s_AudioScratchBuffer, bufferSize, soundFile.samplerate());



	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);

	alSourcef(source, AL_PITCH, 1.0f);
	alSourcePlay(source);


	return false;
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr);
	EDNA_CORE_ASSERT(p_ALCDevice, "Failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	EDNA_CORE_ASSERT(p_ALCContext, "Failed to get sound context");

	auto ContextResult = alcMakeContextCurrent(p_ALCContext);
	EDNA_CORE_ASSERT(ContextResult, "Failed to make current context");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}

	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
	{
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	}

	EDNA_CORE_INFO("Audio: {0}", name);

	s_AudioScratchBuffer = new uint16_t[s_AudioScratchBufferSize];
}

SoundDevice::~SoundDevice()
{

	auto ContextResult = alcMakeContextCurrent(nullptr);
	EDNA_CORE_ASSERT(ContextResult, "Failed to make current context nullptr");

	alcDestroyContext(p_ALCContext);
	EDNA_CORE_ASSERT(!p_ALCContext, "Failed to destroy context");

	auto CloseResult = alcCloseDevice(p_ALCDevice);
	EDNA_CORE_ASSERT(CloseResult, "Failed to close sound device");
}



}