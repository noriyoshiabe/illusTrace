#include "PaintMaskBuilder.h"
#include "cairo/cairo.h"

using namespace illustrace;

static void drawPaths(cairo_t *cr, std::vector<Path *> *paths);
static void drawPath(cairo_t *cr, Path *path);

void PaintMaskBuilder::build(cv::Mat &paintMask, Document *document)
{
    cairo_surface_t *surface = cairo_image_surface_create_for_data(paintMask.data, CAIRO_FORMAT_A8,
            paintMask.cols, paintMask.rows, cairo_format_stride_for_width(CAIRO_FORMAT_A8, paintMask.cols));
    cairo_t *cr = cairo_create(surface);
   
    cairo_set_line_width(cr, document->thickness());
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

    drawPaths(cr, document->paths());

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

// Local functions for Cairo

static void drawPaths(cairo_t *cr, std::vector<Path *> *paths)
{
    for (auto *path : *paths) {
        drawPath(cr, path);

        if (path->closed) {
            cairo_fill_preserve(cr);
        }

        cairo_stroke(cr);
    }
}

static void drawPath(cairo_t *cr, Path *path)
{
    cairo_new_sub_path(cr);

    for (Segment &s : path->segments) {
        switch (s.type) {
        case Segment::Type::Move:
            cairo_move_to(cr, s[2].x, s[2].y);
            break;
        case Segment::Type::Line:
            cairo_line_to(cr, s[2].x, s[2].y);
            break;
        case Segment::Type::Curve:
            cairo_curve_to(cr, s[0].x, s[0].y, s[1].x, s[1].y, s[2].x, s[2].y); 
            break;
        }
    }

    if (path->closed) {
        cairo_close_path(cr);
    }

    for (Path *child : path->children) {
        drawPath(cr, child);
    }
}
