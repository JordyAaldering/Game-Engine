#pragma once

namespace Luci {

	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}
		~Timestep() = default;

		float GetSeconds() const { return m_Time; }
		float GetMilliSeconds() const { return m_Time * 1000.0f; }

		operator float() const { return m_Time; }

	private:
		float m_Time;
	};

}
