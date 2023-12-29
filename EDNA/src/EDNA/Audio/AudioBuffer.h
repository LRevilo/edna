#pragma once

namespace EDNA {

	class AudioBuffer
	{
	public:

		AudioBuffer(uint32_t length, uint32_t channels, float sampleRate);

		float* operator[](uint32_t channel) { return &m_Data[channel * m_Length]; }

	private:
		float m_SampleRate;
		uint32_t m_Length;
		uint32_t m_Channels;
		std::vector<float> m_Data;
	};
}

