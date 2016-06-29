#include "CLI.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <getopt.h>
#include "opencv2/highgui.hpp"
#include "SVGWriter.h"
#include "Log.h"
#include "nalib/NACString.h"

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
        {"color", required_argument, NULL, 'c'},
        {"wait", required_argument, NULL, 'w'},
        {"step", no_argument, NULL, 'S'},
        {"plot", no_argument, NULL, 'p'},
        {"edit", required_argument, NULL, 'e'},
        {"output", required_argument, NULL, 'o'},
        {"trace", no_argument, NULL, 'T'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},

        {NULL, 0, NULL, 0}
    };

    CLI cli;

    int opt;
    while (-1 != (opt = getopt_long(argc, argv, "Ob:B:d:t:s:c:w:Spe:To:hv", _options, NULL))) {
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
        case 'c':
            {
                std::string s = optarg;
                std::regex re("[0-9a-fA-F]{6}(,[0-9a-fA-F]{6})?");

                if (!std::regex_match(s.begin(), s.end(), re)) {
                    std::cout << "Color format is invalid." << std::endl;
                    cli.usage();
                    return EXIT_FAILURE;
                }

                char *background = strchr(optarg, ',');
                if (background) {
                    *background++ = '\0';
                    long hex = std::stol(background, nullptr, 16);
                    auto backgroundColor = cv::Scalar((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0XFF, 255);
                    cli.document->backgroundColor(backgroundColor);
                    cli.document->backgroundEnable(true);
                }

                long hex = std::stol(optarg, nullptr, 16);
                auto color = cv::Scalar((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0XFF, 255);
                cli.document->color(color);
            }
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
        case 'e':
            cli.editFilePath = optarg;
            break;
        case 'o':
            cli.outputFilepath = optarg;
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

CLI::CLI() : editFilePath(nullptr), outputFilepath(nullptr)
{
    document = new Document();
    editor = new Editor(&illustrace, document);
}

CLI::~CLI()
{
    delete editor;
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
        "Usage: illustrace [options] <file>\n"
        "Options:\n"
        "  -O, --outline               Outline trace mode. Default is center line mode.\n"
        "  -b, --brightness <value>    Adjustment for brightness. -1.0 to 1.0.\n"
        "  -B, --blur <value>          Blur size (%% of short side) of the preprocess for binarize. 0.0 to 1.0\n"
        "  -d, --detail <value>        Adjustment for line detail. 0.0 to 1.0.\n"
        "  -t, --thickness <value>     Adjustment for line thickness. 0 < value.\n"
        "  -s, --smooth <value>        Adjustment for bezier smoothness. 0 < value.\n"
        "  -c, --color <color[,color]> Color for line and background(optional).\n"
        "                              Color format is HEX RGB. ex) FF0000,000000\n"
        "  -w, --wait <msec>           Wait milli seconds for each of image proccessing phase.\n"
        "                              0 is infinity and key input is needed for continue.\n"
        "  -S, --step                  Wait with drawing one line.\n"
        "  -p, --plot                  Plot points and handles.\n"
        "  -e, --edit <file>           Edit with command instruction.\n"
        "  -o, --output <file>         Output result to file. Currently, .svg only.\n"
        "  -T, --trace                 Print trace log.\n"
        "  -h, --help                  This help text.\n"
        "  -v, --version               Show program version.\n";

    std::cout << USAGE;
    std::cout << std::endl;
}

void CLI::version()
{
    std::cout << "illustrace CLI Version " << VERSION << " June 11, 2016" << std::endl;
}

bool CLI::execute(const char *inputFilePath)
{
    if (!outputFilepath) {
        illustrace.addObserver(&view);
    }

    bool ret = illustrace.traceFromFile(inputFilePath, document);
    if (!ret) {
        std::cout << "Could not load source image." << std::endl;
        return EXIT_FAILURE;
    }


    if (editFilePath) {
        char str[1024];
        std::ifstream ifs(editFilePath);
        if (ifs.fail()) {
            std::cout << "Could not load command instruction." << std::endl;
            return EXIT_FAILURE;
        }

        illustrace.buildPaintMask(document);

        if (!outputFilepath) {
            illustrace.removeObserver(&view);
            document->addObserver(&view);
            editor->addObserver(&view);
        }

        int line = 0;
        while (ifs.getline(str, 1024 - 1)) {
            executeCommand(str, ++line);
        }
    }

    if (!outputFilepath) {
        view.waitKey();
    }
    else {
        ret = SVGWriter::write(outputFilepath, document, "Generator: illusTrace CLI 0.1.0");
    }

    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}

enum Command {
    Mode,
    PaintState,
    Detail,
    BGEnable,
    Color,
    Draw,
    DrawFinish,
    Fill,
    Undo,
    Redo,

    Unknown,
};

void CLI::executeCommand(char *commandLine, int line)
{
    int argc;
    char **argv = NACStringSplit(commandLine, ": ,", &argc);
    if (0 == argc || '#' == argv[0][0]) {
        return;
    }

    struct {
        const char *name;
        Command command;
        int argc;
    } table[] = {
        {"Mode", Mode, 1},
        {"PaintState", PaintState, 1},
        {"Detail", Detail, 1},
        {"BGEnable", BGEnable, 1},
        {"Color", Color, 3},
        {"Draw", Draw, 2},
        {"DrawFinish", DrawFinish, 0},
        {"Fill", Fill, 2},
        {"Undo", Undo, 0},
        {"Redo", Redo, 0},
    };

    Command command = Unknown;

    for (int i = 0; i < sizeof(table) / sizeof(table)[0]; ++i) {
        if (0 == strcasecmp(table[i].name, argv[0])) {
            if (table[i].argc + 1 > argc) {
                std::cout << "Bad command instruction. line: " << line << std::endl;
                return;
            }
            command = table[i].command;
        }
    }

    switch (command) {
    case Mode:
        {
            struct {
                const char *name;
                Editor::Mode mode;
            } table[] = {
                {"Line", Editor::Mode::Line},
                {"BG", Editor::Mode::BG},
                {"Paint", Editor::Mode::Paint},
                {"Clip", Editor::Mode::Clip},
            };

            for (int i = 0; i < sizeof(table) / sizeof(table)[0]; ++i) {
                if (0 == strcasecmp(table[i].name, argv[1])) {
                    editor->mode(table[i].mode);
                    return;
                }
            }

            std::cout << "Bad command instruction. line: " << line << std::endl;
        }
        break;
    case PaintState:
        {
            struct {
                const char *name;
                Editor::PaintState paintState;
            } table[] = {
                {"Brush", Editor::PaintState::Brush},
                {"Fill", Editor::PaintState::Fill},
                {"Eraser", Editor::PaintState::Eraser},
                {"Color", Editor::PaintState::Color},
            };

            for (int i = 0; i < sizeof(table) / sizeof(table)[0]; ++i) {
                if (0 == strcasecmp(table[i].name, argv[1])) {
                    editor->paintState(table[i].paintState);
                    return;
                }
            }

            std::cout << "Bad command instruction. line: " << line << std::endl;
        }
        break;
    case Detail:
        editor->detail(std::stod(argv[1]));
        break;
    case BGEnable:
        editor->backgroundEnable(0 == strcasecmp("true", argv[1]));
        break;
    case Color:
        editor->R(std::stod(argv[1]));
        editor->G(std::stod(argv[2]));
        editor->B(std::stod(argv[3]));
        break;
    case Draw:
        editor->draw(std::stof(argv[1]), std::stof(argv[2]));
        break;
    case DrawFinish:
        editor->drawFinish();
        break;
    case Fill:
        editor->fill(std::stof(argv[1]), std::stof(argv[2]));
        break;
    case Undo:
        editor->undo();
        break;
    case Redo:
        editor->redo();
        break;
    case Unknown:
        std::cout << "Bad command instruction. line: " << line << std::endl;
        break;
    }
}
