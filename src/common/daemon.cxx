#include <chrono>
#include "daemon.hxx"

using namespace std;
using namespace sevenhearts;

static void daemon_ticker(Daemon *daemon) {
	auto last = chrono::steady_clock::now();
	while (daemon->isRunning()) {
		auto current = chrono::steady_clock::now();
		auto delta = current - last;
		last = current;
		daemon->tick(chrono::duration_cast<chrono::microseconds>(delta).count());
		if (daemon->getDelay()) {
			this_thread::sleep_for(chrono::milliseconds(daemon->getDelay()));
		}
	}
}

Daemon::Daemon(unsigned long msdelay)
		: running(false),
		  delay(msdelay) {
}

Daemon::~Daemon() {
	this->stop();
}

bool Daemon::isRunning() {
	this->running = this->running && this->thread.joinable();
	return this->running;
}

unsigned long Daemon::getDelay() const {
	return this->delay;
}

bool Daemon::start() {
	if (!this->isRunning()) {
		this->running = true;
		this->thread = std::thread(&daemon_ticker, this);
		return true;
	}

	return false;
}

bool Daemon::stop() {
	this->running = false;

	if (this->thread.joinable()) {
		this->thread.join();
		return true;
	}

	return false;
}
