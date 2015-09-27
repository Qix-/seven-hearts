#include <iostream>
#include "log.hxx"

using namespace std;
using namespace sevenhearts;

static const char *ESCAPE = "\x1b[";

const out::Level out::info(100, "INFO");
const out::Level out::error(500, "ERR", "1;31", true);
const out::Level out::warn(250, "WARN", "1;33");
const out::Level out::fatal(1000, "FATAL", "31;1;7", true);
const out::Level out::debug(50, "DEBG", "2", true);
const out::Level out::hack(400, "HACK", "1;35", true);

out::Level::Level(int level, string name, string ansi, bool fullColor)
		: level(level),
		  fullColor(fullColor),
		  name(name),
		  ansi(ansi + "m") {
}

int out::Level::getLevel() const {
	return this->level;
}

bool out::Level::isFullColor() const {
	return this->fullColor;
}

string out::Level::getName() const {
	return this->name;
}

string out::Level::getAnsi() const {
	return this->ansi;
}

out::Level::Entry out::Level::operator ()() const {
	return out::Level::Entry(*this);
}

out::Level::Entry::Entry(const out::Level &level)
		: stringstream(),
		  level(level) {
	*this << ESCAPE << level.getAnsi() << "[" << level.getName() << "]";
	if (!level.isFullColor()) {
		*this << ESCAPE << "0m";
	}
	*this << "\t";
}

out::Level::Entry::Entry(const out::Level::Entry &entry)
		: stringstream(entry.str()),
		  level(entry.level) {
}

out::Level::Entry::~Entry() {
	*this << ESCAPE << "0m" << endl;
	cout << this->str();
	cout.flush();
}
