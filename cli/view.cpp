#include "View.h"
#include "Log.h"

#include <iostream>

using namespace illustrace;

static const char WindowName[] = "illustrace CLI";

View::View() : wait(-1), step(false), plot(false)
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

void View::waitKeyIfNeeded()
{
    if (0 <= wait) {
        cv::waitKey(wait);
    }
}

void View::notify(Illustrace *sender, va_list argList)
{
    Illustrace::Event event = static_cast<Illustrace::Event>(va_arg(argList, int));
    Document *document = va_arg(argList, Document *);

#ifdef DEBUG
    if (__IsTrace__) {
        printf("Illustrace::Event: %s\n", Illustrace::Event2CString(event));
    }
#endif

    switch (event) {
    case Illustrace::Event::SourceImageLoaded:
        {
            cv::Mat *sourceImage = va_arg(argList, cv::Mat *);
            preview = cv::Mat(sourceImage->rows, sourceImage->cols, CV_8UC4);
            surface = cairo_image_surface_create_for_data(preview.data, CAIRO_FORMAT_ARGB32,
                    preview.cols, preview.rows, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, preview.cols));
            cr = cairo_create(surface);
            imshow(WindowName, *sourceImage);
            waitKeyIfNeeded();
        }
        break;
    case Illustrace::Event::BrightnessFilterApplied:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::BlurFilterApplied:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::Binarized:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::Thinned:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::NegativeFilterApplied:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::CenterLineKeyPointDetected:
        if (plot) {
            copyFrom(*va_arg(argList, cv::Mat *));
            plotPoints(*va_arg(argList, std::vector<cv::Point2f> *));
            waitKeyIfNeeded();
        }
        break;
    case Illustrace::Event::CenterLineGraphBuilt:
        if (plot) {
            clearPreview();
            plotGraph(*va_arg(argList, Graph *));
            waitKeyIfNeeded();
        }
        break;
    case Illustrace::Event::CenterLineGraphApproximated:
        if (plot) {
            clearPreview();
            plotGraph(*va_arg(argList, Graph *));
            waitKeyIfNeeded();
        }
        break;
    case Illustrace::Event::CenterLineBuilt:
        {
            auto *centerLines = va_arg(argList, std::vector<std::vector<cv::Point2f>> *);
            clearPreview();
            drawLines(*centerLines, 1);
            waitKeyIfNeeded();
            if (plot) {
                plotPoints(*centerLines);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::CenterLineApproximated:
        {
            auto *approximatedCenterLines = va_arg(argList, std::vector<std::vector<cv::Point2f>> *);
            clearPreview();
            drawLines(*approximatedCenterLines, 1);
            waitKeyIfNeeded();
            if (plot) {
                plotPoints(*approximatedCenterLines);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::CenterLineBezierized:
        {
            auto *paths = va_arg(argList, std::vector<Path *> *);
            if (document->backgroundEnable()) {
                fillBackground(document->backgroundColor());
            }
            else {
                clearPreview();
            }
            drawPaths(paths, document->thickness(), document->color(), document->color());
            waitKeyIfNeeded();
            if (plot) {
                clearPreview();
                auto color = cv::Scalar(128, 128, 128);
                drawPaths(paths, 1, color, color);
                plotPathsHandle(paths);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::OutlineBuilt:
        {
            auto *outlineContours = va_arg(argList, std::vector<std::vector<cv::Point>> *);
            clearPreview();
            drawLines(*outlineContours, 1, true);
            waitKeyIfNeeded();
            if (plot) {
                plotPoints(*outlineContours);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::OutlineApproximated:
        {
            auto *approximatedOutlineContours = va_arg(argList, std::vector<std::vector<cv::Point2f>> *);
            clearPreview();
            drawLines(*approximatedOutlineContours, 1, true);
            waitKeyIfNeeded();
            if (plot) {
                plotPoints(*approximatedOutlineContours);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::OutlineBezierized:
        {
            auto *paths = va_arg(argList, std::vector<Path *> *);
            if (document->backgroundEnable()) {
                fillBackground(document->backgroundColor());
            }
            else {
                clearPreview();
            }
            drawPaths(paths, document->thickness(), document->color(), document->color());
            waitKeyIfNeeded();
            if (plot) {
                clearPreview();
                auto color = cv::Scalar(128, 128, 128);
                drawPaths(paths, 1, color, color);
                plotPathsHandle(paths);
                waitKeyIfNeeded();
            }
        }
        break;
    case Illustrace::Event::PaintMaskBuilt:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::PaintLayerUpdated:
        {
            cv::Mat bgra;
            cv::cvtColor(*va_arg(argList, cv::Mat *), bgra, CV_RGBA2BGRA);
            imshow(WindowName, bgra);
            waitKeyIfNeeded();
        }
        break;
    case Illustrace::Event::PaintPathsBuilt:
        if (document->backgroundEnable()) {
            fillBackground(document->backgroundColor());
        }
        else {
            clearPreview();
        }
        drawPaths(va_arg(argList, std::vector<Path *> *), 0, document->color(), document->color());
        drawPaths(document->paths(), document->thickness(), document->color(), document->color());
        waitKeyIfNeeded();
        break;
    case Illustrace::Event::PreprocessedImageUpdated:
        imshow(WindowName, *va_arg(argList, cv::Mat *));
        waitKeyIfNeeded();
        break;
    }
}

void View::notify(Editor *sender, va_list argList)
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));

#ifdef DEBUG
    if (__IsTrace__) {
        printf("Editor::Event: %s\n", Editor::Event2CString(event));
    }
#endif
}

void View::notify(Document *sender, va_list argList)
{
    Document::Event event = static_cast<Document::Event>(va_arg(argList, int));

#ifdef DEBUG
    if (__IsTrace__) {
        printf("Document::Event: %s\n", Document::Event2CString(event));
    }
#endif
}

void View::clearPreview()
{
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, preview.cols - 1, preview.rows - 1);
    cairo_fill(cr);
    imshow(WindowName, preview);
}

void View::fillBackground(cv::Scalar &color)
{
    cairo_set_source_rgb(cr, color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
    cairo_rectangle(cr, 0, 0, preview.cols - 1, preview.rows - 1);
    cairo_fill(cr);
    imshow(WindowName, preview);
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
    imshow(WindowName, preview);
}

template <class T>
void View::drawLines(std::vector<std::vector<T>> &lines, double thickness, bool closePath)
{
    cairo_set_line_width(cr, thickness);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

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
            waitKeyIfNeeded();
        }
    }

    imshow(WindowName, preview);
}

void View::drawPaths(std::vector<Path *> *paths, double thickness, cv::Scalar &stroke, cv::Scalar &fill)
{
    cairo_set_line_width(cr, thickness);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);

    for (auto *path : *paths) {
        drawPath(path, thickness);

        if (path->closed) {
            if (path->color) {
                cv::Scalar &_color = *path->color;
                cairo_set_source_rgb(cr, _color[0] / 255.0, _color[1] / 255.0, _color[2] / 255.0);
            }
            else {
                cairo_set_source_rgb(cr, fill[0] / 255.0, fill[1] / 255.0, fill[2] / 255.0);
            }
            cairo_fill_preserve(cr);
        }

        if (path->color) {
            cv::Scalar &_color = *path->color;
            cairo_set_source_rgb(cr, _color[0] / 255.0, _color[1] / 255.0, _color[2] / 255.0);
        }
        else {
            cairo_set_source_rgb(cr, stroke[0] / 255.0, stroke[1] / 255.0, stroke[2] / 255.0);
        }

        cairo_stroke(cr);

        if (step) {
            imshow(WindowName, preview);
            waitKeyIfNeeded();
        }
    }

    imshow(WindowName, preview);
}

void View::drawPath(Path *path, double thickness)
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
        drawPath(child, thickness);
    }
}

template <class T>
void View::plotPoints(std::vector<T> &points)
{
    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 1, 0, 0);

    for (auto point : points) {
        cairo_arc(cr, point.x, point.y, 2, 0, 2 * M_PI);
        cairo_stroke(cr);
    }

    imshow(WindowName, preview);
}

template <class T>
void View::plotPoints(std::vector<std::vector<T>> &lines)
{
    for (auto line : lines) {
        plotPoints(line);
    }
}

void View::plotGraph(Graph &graph)
{
    cairo_set_line_width(cr, 1);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);

    for (auto *vertex : graph.vertices) {
        cairo_set_source_rgb(cr, 0, 0, 1);

        for (auto *_vertex : vertex->adjacencyList) {
            cairo_move_to(cr, vertex->point.x, vertex->point.y);
            cairo_line_to(cr, _vertex->point.x, _vertex->point.y);
            cairo_stroke(cr);
        }

        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_arc(cr, vertex->point.x, vertex->point.y, 2, 0, 2 * M_PI);
        cairo_stroke(cr);
    }

    imshow(WindowName, preview);
}

void View::plotPathsHandle(std::vector<Path *> *paths)
{
    cairo_set_line_width(cr, 1);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    for (auto *path : *paths) {
        plotPathHandle(path);
    }

    imshow(WindowName, preview);
}

void View::plotPathHandle(Path *path)
{
    int length = path->segments.size();
    for (int i = 0; i < length; ++i) {
        Segment &s = path->segments[i];

        if (Segment::Type::Curve == s.type) {
            Segment &prev = path->segments[i - 1];

            cairo_set_source_rgba(cr, 0, 0, 1, 0.5);
            cairo_move_to(cr, prev[2].x, prev[2].y);
            cairo_line_to(cr, s[0].x, s[0].y);
            cairo_stroke(cr);

            cairo_move_to(cr, s[1].x, s[1].y);
            cairo_line_to(cr, s[2].x, s[2].y);
            cairo_stroke(cr);

            cairo_arc(cr, s[0].x, s[0].y, 2, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_arc(cr, s[1].x, s[1].y, 2, 0, 2 * M_PI);
            cairo_stroke(cr);
        }

        cairo_set_source_rgba(cr, 1, 0, 0, 0.5);
        cairo_arc(cr, s[2].x, s[2].y, 2, 0, 2 * M_PI);
        cairo_fill(cr);
    }

    for (auto *child : path->children) {
        plotPathHandle(child);
    }
}
