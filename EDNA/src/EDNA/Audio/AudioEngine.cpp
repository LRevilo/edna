#include "ednapch.h"
#include "AudioEngine.h"


namespace EDNA {

	struct AudioData
	{

		uint16_t* s_AudioScratchBuffer;
		uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;
	};
	static AudioData s_AudioData;


	void Audio::Init()
	{

		EDNA_CORE_INFO("Audio: Init");

		s_AudioData.p_ALCDevice = alcOpenDevice(nullptr);
		EDNA_CORE_ASSERT(s_AudioData.p_ALCDevice, "Failed to get sound device");

		s_AudioData.p_ALCContext = alcCreateContext(s_AudioData.p_ALCDevice, nullptr);
		EDNA_CORE_ASSERT(s_AudioData.p_ALCContext, "Failed to get sound context");

		auto ContextResult = alcMakeContextCurrent(s_AudioData.p_ALCContext);
		EDNA_CORE_ASSERT(ContextResult, "Failed to make current context");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(s_AudioData.p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		{
			name = alcGetString(s_AudioData.p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		}

		if (!name || alcGetError(s_AudioData.p_ALCDevice) != AL_NO_ERROR)
		{
			name = alcGetString(s_AudioData.p_ALCDevice, ALC_DEVICE_SPECIFIER);
		}

		EDNA_CORE_INFO("Audio: {0}", name);

		s_AudioData.s_AudioScratchBuffer = new uint16_t[s_AudioData.s_AudioScratchBufferSize];


		// Init listener
		ALfloat listenerPos[] = { 0.0,0.0,0.0 };
		ALfloat listenerVel[] = { 0.0,0.0,0.0 };
		ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);




	}
	void Audio::Close()
	{
		auto ContextResult = alcMakeContextCurrent(nullptr);
		EDNA_CORE_ASSERT(ContextResult, "Failed to make current context nullptr");

		alcDestroyContext(s_AudioData.p_ALCContext);
		EDNA_CORE_ASSERT(!s_AudioData.p_ALCContext, "Failed to destroy context");

		auto CloseResult = alcCloseDevice(s_AudioData.p_ALCDevice);
		EDNA_CORE_ASSERT(CloseResult, "Failed to close sound device");
	}


	Audio::Format Audio::GetFormat(SndfileHandle &fileHandle)
	{
		switch (fileHandle.format())
		{
		case SF_FORMAT_WAV | SF_FORMAT_PCM_32:
			return Audio::Format::WAV32;
		case SF_FORMAT_WAV | SF_FORMAT_PCM_24:
			return Audio::Format::WAV24;
		case SF_FORMAT_WAV | SF_FORMAT_PCM_16:
			return Audio::Format::WAV16;
		case SF_FORMAT_WAV | SF_FORMAT_PCM_S8:
			return Audio::Format::WAV8;
		default:
			EDNA_CORE_TRACE("Invalid format");
			return Audio::Format::None;
		}
	}
	int Audio::GetFormatSize(Format& format)
	{
		switch (format)
		{
		case WAV8:
			return 1;
		case WAV16:
			return 2;
		case WAV24:
			return 3;
		case WAV32:
			return 4;
		case None:
			return 0;
		}
	}



	ALuint Audio::CreateBufferFromFile(const std::string& path)
	{
		SndfileHandle soundFile = SndfileHandle(path);
		EDNA_CORE_TRACE("Channels: {0}", soundFile.channels());
		EDNA_CORE_TRACE("Frames: {0}", soundFile.frames());
		EDNA_CORE_TRACE("Samplerate: {0}", soundFile.samplerate());

		Audio::Format format = GetFormat(soundFile);
		uint32_t formatSize = GetFormatSize(format);
		uint32_t bufferSize = formatSize * soundFile.channels() * soundFile.frames();

		if (s_AudioData.s_AudioScratchBufferSize < bufferSize)
		{
			EDNA_CORE_TRACE("Resizing scratch buffer...");
			s_AudioData.s_AudioScratchBufferSize = bufferSize;
			delete[] s_AudioData.s_AudioScratchBuffer;
			s_AudioData.s_AudioScratchBuffer = new uint16_t[s_AudioData.s_AudioScratchBufferSize];
		}
		auto readResult = soundFile.read((short*)s_AudioData.s_AudioScratchBuffer, soundFile.channels() * soundFile.frames());

		EDNA_CORE_TRACE("Read: {0}", readResult);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, AL_FORMAT_STEREO16, s_AudioData.s_AudioScratchBuffer, bufferSize, soundFile.samplerate());

		return buffer;
	}

	bool Audio::LoadAndPlayFromFile(const std::string& path)
	{	
		SndfileHandle soundFile = SndfileHandle(path);
		EDNA_CORE_TRACE("Channels: {0}", soundFile.channels());
		EDNA_CORE_TRACE("Frames: {0}", soundFile.frames());
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

		if (s_AudioData.s_AudioScratchBufferSize < bufferSize)
		{
			EDNA_CORE_TRACE("Resizing scratch buffer...");
			s_AudioData.s_AudioScratchBufferSize = bufferSize;
			delete[] s_AudioData.s_AudioScratchBuffer;
			s_AudioData.s_AudioScratchBuffer = new uint16_t[s_AudioData.s_AudioScratchBufferSize];
		}
		auto readResult = soundFile.read((short*)s_AudioData.s_AudioScratchBuffer, soundFile.channels() * soundFile.frames());

		EDNA_CORE_TRACE("Read: {0}", readResult);



		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, AL_FORMAT_STEREO16, s_AudioData.s_AudioScratchBuffer, bufferSize, soundFile.samplerate());

		ALuint source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, buffer);

		alSourcef(source, AL_PITCH, 1.0f);
		alSourcePlay(source);


		return true; 
	}
}