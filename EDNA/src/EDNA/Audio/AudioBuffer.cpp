#include "ednapch.h"
#include "AudioBuffer.h"



namespace EDNA {

	AudioBuffer::AudioBuffer(uint32_t length, uint32_t channels, float sampleRate)
		: m_Length(length), m_Channels(channels), m_SampleRate(sampleRate)
	{

		m_Data.assign(static_cast<std::vector<float, std::allocator<float>>::size_type>(length) * channels, 0.0f);

	}


}




