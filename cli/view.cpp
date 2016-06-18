#include "View.h"

#include <iostream>

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
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void View::waitKey()
{
    if (0 != wait) {
        cv::waitKey(0);
    }
}

void View::notify(core::Illustrace *sender, core::IllustraceEvent event, va_list argList)
{
    switch (event) {
    case core::IllustraceEvent::SourceImageLoaded:
        preview = cv::Mat(sender->sourceImage.rows, sender->sourceImage.cols, CV_8UC4);
        surface = cairo_image_surface_create_for_data(preview.data, CAIRO_FORMAT_ARGB32,
                preview.cols, preview.rows, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, preview.cols));
        cr = cairo_create(surface);
        imshow(WindowName, sender->sourceImage);
        break;
    case core::IllustraceEvent::BrightnessFilterApplied:
        imshow(WindowName, sender->binarizedImage);
        break;
    case core::IllustraceEvent::BlurFilterApplied:
        imshow(WindowName, sender->binarizedImage);
        break;
    case core::IllustraceEvent::Binarized:
        imshow(WindowName, sender->binarizedImage);
        break;
    case core::IllustraceEvent::Thinned:
        imshow(WindowName, sender->thinnedImage);
        break;
    case core::IllustraceEvent::NegativeFilterApplied:
        imshow(WindowName, sender->negativeImage);
        break;
    case core::IllustraceEvent::CenterLineKeyPointDetected:
        copyFrom(sender->thinnedImage);
        plotPoints(sender->centerLineKeyPoints);
        break;
    case core::IllustraceEvent::CenterLineBuilt:
        clearPreview();
        drawLines(sender->centerLines, sender->thickness);
        break;
    case core::IllustraceEvent::CenterLineApproximated:
        clearPreview();
        drawLines(sender->approximatedCenterLines, sender->thickness);
        break;
    case core::IllustraceEvent::CenterLineBezierized:
        clearPreview();
        drawBezierLines(sender->bezierizedCenterLines, sender->thickness);
        break;
    case core::IllustraceEvent::OutlineBuilt:
        clearPreview();
        drawLines(sender->outlineContours, sender->thickness, true);
        break;
    case core::IllustraceEvent::OutlineApproximated:
        clearPreview();
        drawLines(sender->approximatedOutlineContours, sender->thickness, true);
        break;
    }

    if (0 <= wait) {
        cv::waitKey(wait);
    }
}

void View::clearPreview()
{
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, preview.cols - 1, preview.rows - 1);
    cairo_fill(cr);
}

void View::copyFrom(cv::Mat &image)
{
    int srcChannel = image.channels();
    int dstChannel = preview.channels();

    for (int y = 0; y < preview.rows; ++y) {
        int yOffsetSrc = y * preview.cols * srcChannel;
        int yOffsetDst = y * preview.cols * dstChannel;
        for (int x = 0; x < preview.cols; ++x) {
            uchar *src = &image.data[yOffsetSrc + (x * srcChannel)];
            uchar *dst = &preview.data[yOffsetDst + (x * dstChannel)];
            if (1 == srcChannel) {
                dst[0] = src[0];
                dst[1] = src[0];
                dst[2] = src[0];
            }
            else {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
            }
        }
    }
}

void View::drawLines(std::vector<std::vector<cv::Point>> lines, double thickness, bool closePath)
{
    cairo_set_line_width(cr, thickness);
    cairo_set_source_rgb(cr, 0, 0, 0);

    for (auto line : lines) {
        int size = line.size();
        
        cairo_move_to(cr, line[0].x, line[0].y);

        if (1 == size) {
            cairo_line_to(cr, line[0].x, line[0].y);
        }
        else {
            for (int i = 1; i < size; ++i) {
                cairo_line_to(cr, line[i].x, line[i].y);
            }
        }

        if (closePath) {
            cairo_line_to(cr, line[0].x, line[0].y);
        }

        cairo_stroke(cr);

        if (step) {
            imshow(WindowName, preview);
            if (0 <= wait) {
                cv::waitKey(wait);
            }
        }
    }

    imshow(WindowName, preview);
}

void View::drawBezierLines(std::vector<std::vector<core::BezierVertex<cv::Point2f>>> bezierLines, double thickness)
{
    cairo_set_line_width(cr, thickness);
    cairo_set_source_rgb(cr, 0, 0, 0);

    for (auto bezierLine : bezierLines) {
        cairo_move_to(cr, bezierLine[0].pt.x, bezierLine[0].pt.y);
        auto ctl1 = bezierLine[0].ctl.next;

        int length = bezierLine.size();
        for (int i = 1; i < length; ++i) {
            auto vtx = bezierLine[i];
            cairo_curve_to(cr, ctl1.x, ctl1.y, vtx.ctl.prev.x, vtx.ctl.prev.y, vtx.pt.x, vtx.pt.y); 
            ctl1 = vtx.ctl.next;
        }

        cairo_stroke(cr);

        if (step) {
            imshow(WindowName, preview);
            if (0 <= wait) {
                cv::waitKey(wait);
            }
        }
    }

    imshow(WindowName, preview);
}

void View::plotPoints(std::vector<cv::Point> points)
{
    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 1, 0, 0);

    for (auto point : points) {
        cairo_arc(cr, point.x, point.y, 2, 0, 2 * M_PI);
        cairo_stroke(cr);
    }

    imshow(WindowName, preview);
}
