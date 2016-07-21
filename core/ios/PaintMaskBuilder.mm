//
//  PaintMaskBuilder.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#include "PaintMaskBuilder.h"

#import <UIKit/UIKit.h>

using namespace illustrace;

static void buildPath(Path *path, CGMutablePathRef subPath);

void PaintMaskBuilder::build(cv::Mat &paintMask, Document *document)
{
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
    CGContextRef context = CGBitmapContextCreate(paintMask.data, paintMask.cols, paintMask.rows, 8, paintMask.cols, colorSpace, kCGImageAlphaNone);

    CGContextSetLineWidth(context, document->thickness());
    CGContextSetLineCap(context, kCGLineCapRound);

    CGContextSetGrayStrokeColor(context, 1.0, 1.0);
    CGContextSetGrayFillColor(context, 1.0, 1.0);
    
    for (auto *path : *document->paths()) {
        CGMutablePathRef pathRef = CGPathCreateMutable();
        
        buildPath(path, pathRef);
        
        if (path->closed) {
            CGContextAddPath(context, pathRef);
            CGContextEOFillPath(context);
        }
        
        CGContextAddPath(context, pathRef);
        CGContextStrokePath(context);
        
        CGPathRelease(pathRef);
    }
    
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
}

static void buildPath(Path *path, CGMutablePathRef subPath)
{
    for (Segment &s : path->segments) {
        switch (s.type) {
            case Segment::Type::Move:
                CGPathMoveToPoint(subPath, NULL, s[2].x, s[2].y);
                break;
            case Segment::Type::Line:
                CGPathAddLineToPoint(subPath, NULL, s[2].x, s[2].y);
                break;
            case Segment::Type::Curve:
                CGPathAddCurveToPoint(subPath, NULL, s[0].x, s[0].y, s[1].x, s[1].y, s[2].x, s[2].y);
                break;
        }
    }
    
    for (Path *child : path->children) {
        buildPath(child, subPath);
    }
}
