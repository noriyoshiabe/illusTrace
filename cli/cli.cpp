#include "CLI.h"

#include <iostream>
#include <string>
#include <getopt.h>
#include "opencv2/highgui.hpp"
#include "Log.h"

using namespace illustrace;

const std::string CLI::VERSION = "0.1.0";

int CLI::main(int argc, char **argv)
{
    static struct option _options[] = {
        {"outline", no_argument, NULL, 'O'},
        {"brightness", required_argument, NULL, 'b'},
        {"blur", required_argument, NULL, 'B'},
        {"detail", required_argument, NULL, 'd'},
        {"thickness", required_argument, NULL, 't'},
        {"smoothing", required_argument, NULL, 's'},
        {"wait", required_argument, NULL, 'w'},
        {"step", no_argument, NULL, 'S'},
        {"plot", no_argument, NULL, 'p'},
        {"trace", no_argument, NULL, 'T'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},

        {NULL, 0, NULL, 0}
    };

    CLI cli;

    int opt;
    while (-1 != (opt = getopt_long(argc, argv, "Ob:B:d:t:s:w:SpThv", _options, NULL))) {
        switch (opt) {
        case 'O':
            cli.document->mode(LineMode::Outline);
            break;
        case 'b':
            cli.document->brightness(std::stod(optarg));
            break;
        case 'B':
            cli.document->blur(std::stod(optarg));
            break;
        case 'd':
            cli.document->detail(std::stod(optarg));
            break;
        case 't':
            cli.document->thickness(std::stod(optarg));
            break;
        case 's':
            cli.document->smoothing(std::stod(optarg));
            break;
        case 'w':
            cli.view.wait = std::stoi(optarg);
            break;
        case 'S':
            cli.view.step = true;
            break;
        case 'p':
            cli.view.plot = true;
            break;
        case 'T':
#ifdef DEBUG
            __IsTrace__ = true;
#else
            std::cout << "Warning: --trace option is only available for debug build." << std::endl;
#endif
            break;
        case 'h':
            cli.help();
            return EXIT_SUCCESS;
        case 'v':
            cli.version();
            return EXIT_SUCCESS;
        case '?':
            cli.usage();
            return EXIT_FAILURE;
        }
    }

    if (argc <= optind) {
        std::cout << "Input file not specified." << std::endl;
        cli.usage();
        return EXIT_FAILURE;
    }

    return cli.execute(argv[optind]) ? EXIT_SUCCESS : EXIT_FAILURE;
}

CLI::CLI()
{
    illustrace.addObserver(&view);
    document = new Document();
}

CLI::~CLI()
{
    delete document;
}

void CLI::help()
{
    const std::string HEADER =
        "illustrace CLI version " + VERSION + "\n"
        "Copyright (c) 2016, Noriyoshi Abe. All Rights Reserved.\n";
    
    std::cout << HEADER;

    usage();
}

void CLI::usage()
{
    std::cout << std::endl;

    const std::string USAGE =
        "Usage: illustrace [options] file\n"
        "Options:\n"
        "  -O, --outline            Outline trace mode. Default is center line mode.\n"
        "  -b, --brightness <value> Adjustment for brightness. -1.0 to 1.0.\n"
        "  -B, --blur <value>       Blur size (%% of short side) of the preprocess for binarize. 0.0 to 1.0\n"
        "  -d, --detail <value>     Adjustment for line detail. 0.0 to 1.0.\n"
        "  -t, --thickness <value>  Adjustment for line thickness. 0 < value.\n"
        "  -s, --smooth <value>     Adjustment for bezier smoothness. 0 < value.\n"
        "  -w, --wait <msec>        Wait milli seconds for each of image proccessing phase.\n"
        "                           0 is infinity and key input is needed for continue.\n"
        "  -S, --step               Wait with drawing one line.\n"
        "  -p, --plot               Plot points and handles.\n"
        "  -T, --trace              Print trace log.\n"
        "  -h, --help               This help text.\n"
        "  -v, --version            Show program version.\n";

    std::cout << USAGE;
    std::cout << std::endl;
}

void CLI::version()
{
    std::cout << "illustrace CLI Version " << VERSION << " June 11, 2016" << std::endl;
}

bool CLI::execute(const char *inputFilePath)
{
    bool ret = illustrace.traceFromFile(inputFilePath, document);
    if (!ret) {
        std::cout << "Could not load source image." << std::endl;
    }
    else {
        view.waitKey();
    }

    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
