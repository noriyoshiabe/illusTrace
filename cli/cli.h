#pragma once

#include <string>

namespace illustrace {
namespace cli {

class CLI {
public:
    static int run(int argc, char **argv);
    static void help();
    static void version();

    static const std::string VERSION; 
};

} // namespace cli
} // namespace illustrace
