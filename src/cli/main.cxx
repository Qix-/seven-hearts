#include "common/log.hxx"

using namespace sevenhearts;

int main(void) {
	int i = 0;
	out::info() << ++i << " this is info level";
	out::error() << ++i << " this is error level";
	out::warn() << ++i << " this is warn level";
	out::fatal() << ++i << " this is fatal level";
	out::debug() << ++i << " this is debug level";
	out::hack() << ++i << " this is hack level";

	return 0;
}
