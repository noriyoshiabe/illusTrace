#include "CLI.h"

#include <iostream>
#include <getopt.h>

using namespace illustrace;
using namespace cli;

const std::string CLI::VERSION = "0.1.0";

int CLI::main(int argc, char **argv)
{
    static struct option _options[] = {
        { "help", no_argument, NULL, 'h'},
        { "version", no_argument, NULL, 'v'},

        {NULL, 0, NULL, 0}
    };

    CLI cli;
    int opt;

    while (-1 != (opt = getopt_long(argc, argv, "hv", _options, NULL))) {
        switch (opt) {
        case 'h':
            cli.help();
            return EXIT_SUCCESS;
        case 'v':
            cli.version();
            return EXIT_SUCCESS;
        case '?':
            return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        if (!cli.loadSourceImage(argv[optind])) {
            return EXIT_FAILURE;
        }
    }
    
    return cli.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}

void CLI::help()
{
    const std::string HEADER =
        "Welcome to the illustrace.\n"
        "\n"
        "illustrace CLI (beta): version " + VERSION + "\n"
        "Copyright (c) 2016, Noriyoshi Abe. All Rights Reserved.\n";

    const std::string USAGE =
        "Usage: illustrace [options] [file]\n"
        "Options:\n"
        "  -h, --help    This help text.\n"
        "  -v, --version Show program version.\n";
    
    std::cout << HEADER;
    std::cout << std::endl;
    std::cout << USAGE;
    std::cout << std::endl;
}

void CLI::version()
{
    std::cout << "illustrace CLI Version " << VERSION << " June 11, 2016" << std::endl;
}

bool CLI::loadSourceImage(const char *filename)
{
    return illustrace.loadSourceImage(filename);
}

bool CLI::run()
{
    return true;
}
