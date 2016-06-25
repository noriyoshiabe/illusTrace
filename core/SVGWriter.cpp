#include "SVGWriter.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include <sstream>

using namespace illustrace;

static void writePathToStringStream(Path *path, std::stringstream &ss)
{
    for (Segment &s : path->segments) {
        switch (s.type) {
        case Segment::Type::Move:
            ss << "M" << s[2].x << "," << s[2].y;
            break;
        case Segment::Type::Line:
            ss << " L" << s[2].x << "," << s[2].y;
            break;
        case Segment::Type::Curve:
            ss << " C" << s[0].x << "," << s[0].y << " " << s[1].x << "," << s[1].y << " " << s[2].x << "," << s[2].y;
            break;
        }
    }

    if (path->closed) {
        ss << " Z";
    }

    for (Path *child : path->children) {
        ss << " ";
        writePathToStringStream(child, ss);
    }
}

bool SVGWriter::write(const char *filepath, Document *document, const char *comment)
{
    int ret = 0;
    char str[128];

    xmlTextWriterPtr writer = xmlNewTextWriterFilename(filepath, 0);
    if (!writer) {
        return false;
    }

#define CHECK_AND_ABORT if (-1 == ret) { xmlFreeTextWriter(writer); return false; }

    ret = xmlTextWriterSetIndent(writer, 1);
    CHECK_AND_ABORT;
    ret = xmlTextWriterSetIndentString(writer, BAD_CAST "    ");
    CHECK_AND_ABORT;
    ret = xmlTextWriterStartDocument(writer, NULL, "UTF-8", "no");
    CHECK_AND_ABORT;
    ret = xmlTextWriterStartElement(writer, BAD_CAST "svg");
    CHECK_AND_ABORT;

    cv::Rect &clippingRect = document->clippingRect();

    sprintf(str, "%dpx", clippingRect.width);
    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "width", BAD_CAST str);
    CHECK_AND_ABORT;

    sprintf(str, "%dpx", clippingRect.height);
    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "height", BAD_CAST str);
    CHECK_AND_ABORT;

    sprintf(str, "%d %d %d %d", clippingRect.x, clippingRect.y, clippingRect.width, clippingRect.height);
    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "viewBox", BAD_CAST str);
    CHECK_AND_ABORT;

    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "1.1");
    CHECK_AND_ABORT;
    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/2000/svg");
    CHECK_AND_ABORT;
    ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
    CHECK_AND_ABORT;

    if (document->backgroundEnable()) {
        cv::Scalar &backgroundColor = document->backgroundColor();
        sprintf(str, "background: #%02X%02X%02X;", (int)backgroundColor[0], (int)backgroundColor[1], (int)backgroundColor[2]);
        ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "style", BAD_CAST str);
        CHECK_AND_ABORT;
    }

    {
        if (comment) {
            sprintf(str, " %s ", comment);
            ret = xmlTextWriterWriteComment(writer, BAD_CAST str);
            CHECK_AND_ABORT;
        }

        ret = xmlTextWriterStartElement(writer, BAD_CAST "g");
        CHECK_AND_ABORT;

        cv::Scalar &color = document->color();
        sprintf(str, "#%02X%02X%02X", (int)color[0], (int)color[1], (int)color[2]);
        ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "stroke", BAD_CAST str);
        CHECK_AND_ABORT;

        if (0.1 > fabs(round(document->thickness()) - document->thickness())) {
            sprintf(str, "%d", (int)round(document->thickness()));
        }
        else {
            sprintf(str, "%.1f", document->thickness());
        }

        ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "stroke-width", BAD_CAST str);
        CHECK_AND_ABORT;
        ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "stroke-linecap", BAD_CAST "round");
        CHECK_AND_ABORT;

        if (LineMode::Outline == document->mode()) {
            ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "fill-rule", BAD_CAST "evenodd");
            CHECK_AND_ABORT;

            sprintf(str, "#%02X%02X%02X", (int)color[0], (int)color[1], (int)color[2]);
            ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "fill", BAD_CAST str);
            CHECK_AND_ABORT;
        }
        else {
            ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "fill", BAD_CAST "none");
            CHECK_AND_ABORT;
        }

        for (auto *path : *document->paths()) {
            std::stringstream ss = std::stringstream("");

            ret = xmlTextWriterStartElement(writer, BAD_CAST "path");
            CHECK_AND_ABORT;

            writePathToStringStream(path, ss);

            ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "d", BAD_CAST ss.str().c_str());
            CHECK_AND_ABORT;
            ret = xmlTextWriterFullEndElement(writer);
            CHECK_AND_ABORT;
        }

        ret = xmlTextWriterFullEndElement(writer);
        CHECK_AND_ABORT;
    }

    ret = xmlTextWriterFullEndElement(writer);
    CHECK_AND_ABORT;

    ret = xmlTextWriterEndDocument(writer);
    CHECK_AND_ABORT;

    xmlFreeTextWriter(writer);

    return 0 == ret;
}
