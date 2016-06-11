#pragma once

#include <string>
#include "View.h"
#include "Illustrace.h"

namespace illustrace {
namespace cli {

class CLI {
public:
    static int main(int argc, char **argv);
    static const std::string VERSION;

    CLI();

    void help();
    void usage();
    void version();
    bool execute(const char *inputFilePath);

    View view;
    core::Illustrace illustrace;
};

} // namespace cli
} // namespace illustrace
