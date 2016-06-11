#pragma once

#include <string>
#include "Illustrace.h"

namespace illustrace {
namespace cli {

class CLI {
public:
    static int main(int argc, char **argv);
    static const std::string VERSION;

    void help();
    void version();
    bool run();
    bool loadSourceImage(const char *filename);

private:
    core::Illustrace illustrace;
};

} // namespace cli
} // namespace illustrace
