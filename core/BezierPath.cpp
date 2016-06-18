#include "BezierPath.h"

using namespace illustrace;
using namespace core;

BezierPath::BezierPath(cv::Mat &image, cv::Scalar color, float thickness)
{
    surface = cairo_image_surface_create_for_data(
            image.data, CAIRO_FORMAT_ARGB32, image.cols, image.rows, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, image.cols));
    cr = cairo_create(surface);
    cairo_set_line_width(cr, thickness);
    cairo_set_source_rgb(cr, color[2], color[1], color[0]);
}

BezierPath::~BezierPath()
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void BezierPath::moveToPoint(const cv::Point2f &point)
{
    cairo_move_to(cr, point.x, point.y);
}

void BezierPath::curveToPoint(const cv::Point2f &point, const cv::Point2f &control1, const cv::Point2f &control2)
{
    cairo_curve_to(cr, control1.x, control1.y, control2.x, control2.y, point.x, point.y);
}

void BezierPath::stroke()
{
    cairo_stroke(cr);
}
