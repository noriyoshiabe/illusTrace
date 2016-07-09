//
//  EditViewPreviewView.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditViewPreviewView.h"

using namespace illustrace;

@interface EditViewPreviewView() {
    CGAffineTransform _identity;
}
@end

@implementation EditViewPreviewView

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    auto contentRect = _document->contentRect();
    CGFloat contentAspect = (CGFloat)contentRect.height / (CGFloat)contentRect.width;
    CGFloat viewAspect = self.bounds.size.height / self.bounds.size.width;
    
    CGFloat scale = contentAspect < viewAspect ? self.bounds.size.height / contentRect.height : self.bounds.size.width / contentRect.width;
    CGAffineTransform _scale = CGAffineTransformMakeScale(scale, scale);
    CGAffineTransform _translation = CGAffineTransformMakeTranslation((self.bounds.size.width - contentRect.width * scale) / 2.0, (self.bounds.size.height - contentRect.height * scale) / 2.0);
    
    _identity = CGAffineTransformConcat(_scale, _translation);
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextConcatCTM(context, _identity);
    
    [self drawPaths:context];
    
    CGContextClipToRect(context, rect);
}

- (void)drawPaths:(CGContextRef)context
{
    CGContextSetRGBStrokeColor(context, 0.0, 0.0, 0.0, 1.0);
    CGContextSetRGBFillColor(context, 0.0, 0.0, 0.0, 1.0);
    
    CGContextSetLineWidth(context, _document->thickness());
    CGContextSetLineCap(context, kCGLineCapRound);
    
    for (auto *path : *_document->paths()) {
        CGMutablePathRef pathRef = CGPathCreateMutable();
        
        [self drawPath:path subPath:pathRef];
        
        if (path->closed) {
            CGContextAddPath(context, pathRef);
            CGContextEOFillPath(context);
        }
        
        CGContextAddPath(context, pathRef);
        CGContextStrokePath(context);
        
        CGPathRelease(pathRef);
    }
}

- (void)drawPath:(Path *)path subPath:(CGMutablePathRef)subPath
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
        [self drawPath:child subPath:subPath];
    }
}

@end
