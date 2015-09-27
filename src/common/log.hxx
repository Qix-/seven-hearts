#ifndef SVH_COMMON_LOG_H__
#define SVH_COMMON_LOG_H__
#pragma once

#include <string>

namespace sevenhearts {

	namespace Log {

		class Level {
		public:
			static const Level INFO;

			Level(int level, std::string name, std::string ansi = "", bool fullColor = false);

			int getLevel() const;
			bool isFullColor() const;
			std::string getName() const;
			std::string getAnsi() const;

		private:
			int level;
			bool fullColor;
			std::string name;
			std::string ansi;
		};

	}

}

#endif
