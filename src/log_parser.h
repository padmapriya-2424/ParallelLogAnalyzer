#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include <string>
#include "stats.h"

void processLine(const std::string& line, Stats& stats);

#endif
