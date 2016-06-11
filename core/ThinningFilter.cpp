#include "ThinningFilter.h" 

#include <cstring>

using namespace illustrace;
using namespace core;

void ThinningFilter::apply(cv::Mat &image)
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
                    // [p9 p2 p3 pA]
                    // [p8 p1 p4 pB]
                    // [p7 p6 p5 pC]
                    // [pG pF pE pD]

                    int p1 = 1;
                    int p2 = 0 == prevRow[x  ] ? 1 : 0;
                    int p3 = 0 == prevRow[x+1] ? 1 : 0;
                    int p4 = 0 == currRow[x+1] ? 1 : 0;
                    int p5 = 0 == nextRow[x+1] ? 1 : 0;
                    int p6 = 0 == nextRow[x  ] ? 1 : 0;
                    int p7 = 0 == nextRow[x-1] ? 1 : 0;
                    int p8 = 0 == currRow[x-1] ? 1 : 0;
                    int p9 = 0 == prevRow[x-1] ? 1 : 0;

                    // For avoid vanish point with 4 pixels square
                    if (p1 && p4 && p6 && p5) {
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
                    if (!p2 && p3) ++a;
                    if (!p3 && p4) ++a;
                    if (!p4 && p5) ++a;
                    if (!p5 && p6) ++a;
                    if (!p6 && p7) ++a;
                    if (!p7 && p8) ++a;
                    if (!p8 && p9) ++a;
                    if (!p9 && p2) ++a;

                    int b = p2 + p3 + p4 + p5 + p6 + p7 + p8 +p9;

                    if (a == 1 && 2 <= b && b <= 6) {
                        if ((!(k & 1) && p2*p4*p6 == 0 && p4*p6*p8 == 0)
                            || ((k & 1) && p2*p4*p8 == 0 && p2*p6*p8 == 0)) {
                            currImageRow[x] = 255;
                            flag = true;
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
