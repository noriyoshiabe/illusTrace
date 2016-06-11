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
    void version();
    bool run();
    bool loadSourceImage(const char *filename);

private:
    View view;
    core::Illustrace illustrace;
};

} // namespace cli
} // namespace illustrace
