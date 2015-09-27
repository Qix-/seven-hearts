#ifndef SVH_COMMON_LOG_H__
#define SVH_COMMON_LOG_H__
#pragma once

#include <string>
#include <sstream>

namespace sevenhearts {

	namespace out {

		class Level {
		public:
			Level(int level, std::string name, std::string ansi = "0", bool fullColor = false);

			int getLevel() const;
			bool isFullColor() const;
			std::string getName() const;
			std::string getAnsi() const;

			class Entry : public std::stringstream {
			public:
				Entry(const Level &level);
				Entry(const Entry &entry);
				virtual ~Entry();

			private:
				const Level &level;
			};

			Entry operator ()() const;

		private:
			int level;
			bool fullColor;
			std::string name;
			std::string ansi;
		};

		extern const Level info;
		extern const Level error;
		extern const Level warn;
		extern const Level fatal;
		extern const Level debug;
		extern const Level hack;
	}

}

#endif
