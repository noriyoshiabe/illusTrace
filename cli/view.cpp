#include "View.h"

#include <iostream>
#include "opencv2/highgui.hpp"

using namespace illustrace;
using namespace cli;

static const char WindowName[] = "illustrace CLI";

View::View() : wait(-1)
{
    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);
}

View::~View()
{
    cv::destroyWindow(WindowName);
}

void View::notify(core::Illustrace *sender, core::IllustraceEvent event, va_list argList)
{
    switch (event) {
    case core::IllustraceEvent::SourceImageLoaded:
        imshow(WindowName, sender->getSourceImage());
        break;
    case core::IllustraceEvent::BrightnessChanged:
        imshow(WindowName, sender->getBrightnessChangedImage());
        break;
    }

    if (0 <= wait) {
        cv::waitKey(wait);
    }
}
