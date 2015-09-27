#include <iostream>
#include "log.hxx"

using namespace std;
using namespace sevenhearts;

static const char *ESCAPE = "\x1b[";

const out::Level out::info(100, "INFO");

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
	*this << (!level.isFullColor() ? ESCAPE + level.getAnsi() : "") << "\t";
}

out::Level::Entry::Entry(const out::Level::Entry &entry)
		: stringstream(entry.str()),
		  level(entry.level) {
}

out::Level::Entry::~Entry() {
	*this << ESCAPE << "0m" << endl;
	cout << this->str();
}
