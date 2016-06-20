#pragma once

#include <string>
#include "View.h"
#include "Illustrace.h"

namespace illustrace {

class CLI {
public:
    static int main(int argc, char **argv);
    static const std::string VERSION;

    CLI();
    ~CLI();

    void help();
    void usage();
    void version();
    bool execute(const char *inputFilePath);

    Document *document;
    View view;
    Illustrace illustrace;
    bool outline;
};

} // namespace illustrace
