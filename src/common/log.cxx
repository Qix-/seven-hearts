#include "log.hxx"

using namespace std;
using namespace sevenhearts;

const Log::Level Log::Level::INFO(100, "INFO");

Log::Level::Level(int level, string name, string ansi, bool fullColor)
		: level(level),
		  name(name),
		  ansi(ansi),
		  fullColor(fullColor) {
}

int Log::Level::getLevel() const {
	return this->level;
}

bool Log::Level::isFullColor() const {
	return this->fullColor;
}

string Log::Level::getName() const {
	return this->name;
}

string Log::Level::getAnsi() const {
	return this->ansi;
}
