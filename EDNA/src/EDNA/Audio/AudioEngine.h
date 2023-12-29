#pragma once
#include "sndfile.hh"
#include "AL/al.h"
#include "AL/alc.h"

namespace EDNA {

	class Audio
	{
	private:


		enum Format
		{
			None = 0, WAV8, WAV16, WAV24, WAV32
		};

	public:

		static void Init();
		static void Close();
		static Format GetFormat(SndfileHandle& fileHandle);
		static int GetFormatSize(Format& format);

		static ALuint CreateBufferFromFile(const std::string& path);
		static bool LoadAndPlayFromFile(const std::string& path);


	};

}