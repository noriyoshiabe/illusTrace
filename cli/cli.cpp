#include "CLI.h"

#include <iostream>
#include <string>
#include <getopt.h>
#include "opencv2/highgui.hpp"

using namespace illustrace;
using namespace cli;

const std::string CLI::VERSION = "0.1.0";

int CLI::main(int argc, char **argv)
{
    static struct option _options[] = {
        {"outline", no_argument, NULL, 'O'},
        {"brightness", required_argument, NULL, 'b'},
        {"blur", required_argument, NULL, 'B'},
        {"wait", required_argument, NULL, 'w'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},

        {NULL, 0, NULL, 0}
    };

    CLI cli;
    cli.illustrace.plotKeyPoints = true;

    int opt;
    while (-1 != (opt = getopt_long(argc, argv, "Ob:B:w:hv", _options, NULL))) {
        switch (opt) {
        case 'O':
            cli.outline = true;
            break;
        case 'b':
            cli.illustrace.brightnessFilter.brightness = std::stod(optarg);
            break;
        case 'B':
            {
                int blur = std::stoi(optarg);
                if (0 > blur || 0 == blur % 2) {
                    std::cout << "-B <blur> must be sined odd number." << std::endl;
                    return EXIT_SUCCESS;
                }
                cli.illustrace.blurFilter.blur = blur;
            }
            break;
        case 'w':
            cli.view.wait = std::stoi(optarg);
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
        "  -B, --blur <size>        Blur size of the preprocess for binarize.\n"
        "  -w, --wait <msec>        Wait milli seconds for each of image proccessing phase.\n"
        "                           0 is infinity and key input is needed for continue.\n"
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
    bool ret = illustrace.loadSourceImage(inputFilePath);
    if (!ret) {
        std::cout << "Could not load source image." << std::endl;
        goto ERROR;
    }

    illustrace.binarize();

    if (outline) {
        illustrace.buildOutline();
    }
    else {
        illustrace.thin();
        illustrace.buildCenterLine();
    }

    if (0 != view.wait) {
        cv::waitKey(0);
    }

ERROR:
    return ret;
}
