#include "Filter.h" 

using namespace illustrace;

void Filter::brightness(cv::Mat &image, double brightness, double contrast)
{
    brightness *= 255.0;

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = cv::saturate_cast<uchar>(contrast * image.data[i] + brightness);
    }
}

void Filter::blur(cv::Mat &image, int blur)
{
    cv::GaussianBlur(image, image, cv::Size(blur, blur), 0, 0);
}

void Filter::threshold(cv::Mat &image)
{
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void Filter::negative(cv::Mat &image)
{
    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = 255 - image.data[i];
    }
}

void Filter::thinning(cv::Mat &image)
{
    // Based on `A note on the Nagendraprasad-Wang-Gupta thinning algorithm`
    // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.2.6996&rep=rep1&type=pdf

    int width = image.cols;
    int height = image.rows;
    int length = width * height;
    int xRangeMax = width - 1;
    int yRangeMax = height - 1;

    uchar *snap = new uchar[length];

    for (int k = 0; k < 100; ++k) {
        bool flag = false;
        memcpy(snap, image.data, length);

        for (int y = 1; y < yRangeMax; ++y) {
            int offset = y * image.cols;
            uchar *prevRow = snap + offset - image.cols;
            uchar *currRow = snap + offset;
            uchar *nextRow = snap + offset + image.cols;
            uchar *afterNextRow = snap + offset + image.cols * 2;
            uchar *currImageRow = image.data + offset;

            for (int x = 1; x < xRangeMax; ++x) {
                if (0 == currRow[x]) {
                    // [p7 p0 p1 pA]
                    // [p6    p2 pB]
                    // [p5 p4 p3 pC]
                    // [pG pF pE pD]

                    int p0 = 0 == prevRow[x  ] ? 1 : 0;
                    int p1 = 0 == prevRow[x+1] ? 1 : 0;
                    int p2 = 0 == currRow[x+1] ? 1 : 0;
                    int p3 = 0 == nextRow[x+1] ? 1 : 0;
                    int p4 = 0 == nextRow[x  ] ? 1 : 0;
                    int p5 = 0 == nextRow[x-1] ? 1 : 0;
                    int p6 = 0 == currRow[x-1] ? 1 : 0;
                    int p7 = 0 == prevRow[x-1] ? 1 : 0;

                    // For avoid vanish point with 4 pixels square
                    if (p2 && p4 && p3) {
                        if (y + 2 < height && x + 2 < width) {
                            int pA = 0 == prevRow[x+2] ? 1 : 0;
                            int pB = 0 == currRow[x+2] ? 1 : 0;
                            int pC = 0 == nextRow[x+2] ? 1 : 0;
                            int pD = 0 == afterNextRow[x+2] ? 1 : 0;
                            int pE = 0 == afterNextRow[x+1] ? 1 : 0;
                            int pF = 0 == afterNextRow[x  ] ? 1 : 0;
                            int pG = 0 == afterNextRow[x-1] ? 1 : 0;

                            if (!pA && !pB && !pC && !pD && !pE && !pF && !pG) {
                                continue;
                            }
                        }
                    }

                    int a = 0;
                    if (!p0 && p1) ++a;
                    if (!p1 && p2) ++a;
                    if (!p2 && p3) ++a;
                    if (!p3 && p4) ++a;
                    if (!p4 && p5) ++a;
                    if (!p5 && p6) ++a;
                    if (!p6 && p7) ++a;
                    if (!p7 && p0) ++a;

                    int b = p0 + p1 + p2 + p3 + p4 + p5 + p6 +p7;

                    if (2 <= b && b <= 6) {
                        int c = 0;
                        if ((p0 + p1 + p2 +p5 == 0 && p4 + p6 == 2)
                                || (p2 + p3 + p4 + p7 == 0 && p0 + p6 == 2)) {
                            c = 1;
                        }

                        if (a == 1 || c == 1){
                            int e = (p2 + p4) * p0 * p6;
                            int f = (p0 + p6) * p2 * p4;
                            if ((!(k & 1) && e == 0)
                                    || ( (k & 1) && f == 0)) {
                                currImageRow[x] = 255;
                                flag = true;
                            }
                        }
                    }
                }
            }
        }
        
        if (!flag) {
            break;
        }
    }

    delete[] snap;
}
