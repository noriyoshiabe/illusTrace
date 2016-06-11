#include "View.h"

#include <iostream>
#include "opencv2/highgui.hpp"

using namespace illustrace;
using namespace cli;

static const char WindowName[] = "illustrace CLI";

View::View()
{
    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);
}

View::~View()
{
    cv::destroyWindow(WindowName);
}

void View::notify(core::Illustrace *sender, core::IllustraceEvent event, va_list argList)
{
    imshow(WindowName, *va_arg(argList, cv::Mat *));
    cv::waitKey(0);
}
