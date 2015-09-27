#ifndef SVH_COMMON_DAEMON_H__
#define SVH_COMMON_DAEMON_H__
#pragma once

#include <thread>

namespace sevenhearts {

	class Daemon {
	public:
		Daemon(unsigned long msdelay = 0);
		Daemon(const Daemon&) = delete;
		virtual ~Daemon();

		virtual void tick(unsigned long delta) = 0;

		bool start();
		bool stop();

		bool isRunning();
		unsigned long getDelay() const;

	private:
		std::thread thread;
		bool running;
		const unsigned long delay;
	};

}

#endif
