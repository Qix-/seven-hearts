#include <chrono> // XXX DEBUG
#include "common/log.hxx"
#include "common/daemon.hxx"

using namespace std;
using namespace sevenhearts;

class TestDaemon : public Daemon {
public:
	TestDaemon() : Daemon(1) {
	}

	void tick(unsigned long delta) {
		out::debug() << (double) delta / 1000.0 << "us";
	}
};

int main(void) {
	out::info() << "creating daemon...";
	TestDaemon daemon;
	out::info() << "daemon created.";
	out::info() << "daemon starting...";
	if (!daemon.start()) {
		out::warn() << "it doesn't appear the daemon started";
	} else {
		out::info() << "daemon started!";
	}
	out::info() << "sleeping for 3 seconds...";
	this_thread::sleep_for(chrono::seconds(3));
	out::info() << "attempting to stop daemon...";
	if (!daemon.stop()) {
		out::warn() << "it doesn't appear the daemon stopped";
	} else {
		out::info() << "daemon stopped!";
	}

	return 0;
}
