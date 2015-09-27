#include "common/log.hxx"

using namespace sevenhearts;

int main(void) {
	out::info() << "Hello, " << std::string("Qix") << "!";
	out::info() << "It's great to have you today!";

	return 0;
}
