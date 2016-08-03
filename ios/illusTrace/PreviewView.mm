//
//  PreviewView.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "PreviewView.h"
#import "DocumentObserver.h"

using namespace illustrace;

@interface PreviewView() <DocumentObserver> {
    CGAffineTransform _identity;
    CGAffineTransform _transform;
    
    struct {
        CGAffineTransform transform;
        CGPoint point;
        CGPoint pinchCenter;
        CGPoint velocity;
        CFAbsoluteTime time;
    } _last;
    
    bool _panning;
    bool _pinching;
    
    UIPanGestureRecognizer *_panGestureRecognizer;
    UIPinchGestureRecognizer *_pinchGestureRecognizer;
    
    DocumentObserverBridge _documentObserverbridge;
}
@end

@implementation PreviewView

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    _panGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGestureRecognizerAction:)];
    [self addGestureRecognizer:_panGestureRecognizer];
    
    _pinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchGestureRecognizerAction:)];
    [self addGestureRecognizer:_pinchGestureRecognizer];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    auto contentRect = _document->contentRect();
    CGFloat scale = self.bounds.size.width / contentRect.width;
    CGAffineTransform scaleT = CGAffineTransformMakeScale(scale, scale);
    CGAffineTransform translationT = CGAffineTransformMakeTranslation((self.bounds.size.width - contentRect.width * scale) / 2.0, (self.bounds.size.height - contentRect.height * scale) / 2.0);
    
    _identity = CGAffineTransformConcat(scaleT, translationT);
    _transform = _identity;
    
    _documentObserverbridge.observer = self;
    _document->addObserver(&_documentObserverbridge);
}

- (void)setDrawPreprocessedImage:(BOOL)drawPreprocessedImage
{
    _drawPreprocessedImage = drawPreprocessedImage;
    [self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextConcatCTM(context, _transform);
    
    if (_drawPreprocessedImage) {
        [self drawPreprocessedImage:context];
    }
    else {
        [self drawPaths:context];
    }
    
    CGContextClipToRect(context, rect);
    
    CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
    [self inertia:time - _last.time];
    _last.time = time;
}

- (void)drawPaths:(CGContextRef)context
{
    auto &color = _document->color();
    CGFloat r = color[0] / 255.0;
    CGFloat g = color[1] / 255.0;
    CGFloat b = color[2] / 255.0;
    
    CGContextSetRGBStrokeColor(context, r, g, b, 1.0);
    CGContextSetRGBFillColor(context, r, g, b, 1.0);
    
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

- (void)drawPreprocessedImage:(CGContextRef)context
{
    auto &_image = _document->preprocessedImage();
    CFDataRef data = CFDataCreate(NULL, _image.data, _image.step[0] * _image.rows);
    CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
    CGImageRef image = CGImageCreate(_image.cols, _image.rows, 8, 8, _image.step[0], colorSpace, kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);
    
    CGContextConcatCTM(context, CGAffineTransformMake(1, 0, 0, -1, 0, _image.rows));
    
    CGRect rect = CGRectMake(0, 0, _image.cols, _image.rows);
    CGContextClipToMask(context, rect, image);
    CGContextSetRGBFillColor(context, 0, 0, 0, 1.0);
    CGContextFillRect(context, rect);
    
    CGImageRelease(image);
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(provider);
    CFRelease(data);
}

- (void)inertia:(CFAbsoluteTime)delta
{
    bool needsDisplay = NO;
    
    if (1 <= fabs(_last.velocity.x) || 1 <= fabs(_last.velocity.y)) {
        const CGFloat VELOCITY_DECAY = 0.9;
        
        CGFloat f = (CGFloat)delta / 1.0;
        
        CGAffineTransform t = CGAffineTransformMakeTranslation(_last.velocity.x * f, _last.velocity.y * f);
        _transform = CGAffineTransformConcat(_transform, t);
        
        _last.velocity.x *= VELOCITY_DECAY;
        _last.velocity.y *= VELOCITY_DECAY;
        
        needsDisplay = YES;
    }
    
    CGAffineTransform normlized = CGAffineTransformConcat(_transform, CGAffineTransformInvert(_identity));
    
    if (!_pinching) {
        const CGFloat SCALE_DECAY = 0.9;
        
        if (1.0 > normlized.a) {
            if (0.99 < normlized.a) {
                _transform = _identity;
            }
            else {
                _transform.a = _identity.a + (_transform.a - _identity.a) * SCALE_DECAY;
                _transform.b = _identity.b + (_transform.b - _identity.b) * SCALE_DECAY;
                _transform.c = _identity.c + (_transform.c - _identity.c) * SCALE_DECAY;
                _transform.d = _identity.d + (_transform.d - _identity.d) * SCALE_DECAY;
                _transform.tx = _identity.tx + (_transform.tx - _identity.tx) * SCALE_DECAY;
                _transform.ty = _identity.ty + (_transform.ty - _identity.ty) * SCALE_DECAY;
            }
            
            needsDisplay = YES;
        }
        else if (3.0 < normlized.a) {
            if (3.01 > normlized.a) {
                _transform.a = _identity.a * 3.0;
                _transform.d = _identity.d * 3.0;
            }
            else {
                CGAffineTransform t = CGAffineTransformIdentity;
                t = CGAffineTransformTranslate(t, _last.point.x, _last.point.y);
                t = CGAffineTransformScale(t, SCALE_DECAY, SCALE_DECAY);
                t = CGAffineTransformTranslate(t, -_last.point.x, -_last.point.y);
                
                _transform = CGAffineTransformConcat(_transform, t);
            }
            
            needsDisplay = YES;
        }
    }
    
    if (!_panning && 1.0 <= normlized.a) {
        const CGFloat SCROLL_DECAY = 0.8;
        
        auto contentRect = _document->contentRect();
        CGPoint topLeft = CGPointApplyAffineTransform(CGPointZero, _transform);
        CGPoint bottomRight = CGPointApplyAffineTransform(CGPointMake(contentRect.width, contentRect.height), _transform);
        
        CGFloat height1x = self.bounds.size.width * ((CGFloat)contentRect.height / (CGFloat)contentRect.width);
        CGFloat top = MAX(0.0, self.center.y - height1x / 2.0 * normlized.a);
        CGFloat left = 0.0;
        CGFloat right = self.bounds.size.width;
        CGFloat bottom = MIN(self.bounds.size.height, self.center.y + height1x / 2.0 * normlized.a);
        
        CGPoint translate = CGPointZero;
        
        if (left < topLeft.x) {
            CGFloat decay = left + 0.01 > topLeft.x ? 1.0 : 1.0 - SCROLL_DECAY;
            translate.x = (left - topLeft.x) * decay;
        }
        else if (right > bottomRight.x) {
            CGFloat decay = right - 0.01 < bottomRight.x ? 1.0 : 1.0 - SCROLL_DECAY;
            translate.x = (right - bottomRight.x) * decay;
        }
        
        if (top < topLeft.y) {
            CGFloat decay = top + 0.01 > topLeft.y ? 1.0 : 1.0 - SCROLL_DECAY;
            translate.y = (top - topLeft.y) * decay;
        }
        else if (bottom > bottomRight.y) {
            CGFloat decay = bottom - 0.01 < bottomRight.y ? 1.0 : 1.0 - SCROLL_DECAY;
            translate.y = (bottom - bottomRight.y) * decay;
        }
        
        if (!CGPointEqualToPoint(translate, CGPointZero)) {
            CGAffineTransform t = CGAffineTransformMakeTranslation(translate.x, translate.y);
            _transform = CGAffineTransformConcat(_transform, t);
            needsDisplay = YES;
        }
    }
    
    if (needsDisplay) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setNeedsDisplay];
        });
    }
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    [_delegate previewView:self touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    [_delegate previewView:self touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
    [_delegate previewView:self touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];
    [_delegate previewView:self touchesCancelled:touches withEvent:event];
}

- (BOOL)scrollEnabled
{
    return _panGestureRecognizer.enabled;
}

- (void)setScrollEnabled:(BOOL)scrollEnabled
{
    _panGestureRecognizer.enabled = scrollEnabled;
}

- (BOOL)zoomEnabled
{
    return _pinchGestureRecognizer.enabled;
}

- (void)setZoomEnabled:(BOOL)zoomEnabled
{
    _pinchGestureRecognizer.enabled = zoomEnabled;
}

- (CGPoint)locationInDocument:(CGPoint)point
{
    return CGPointApplyAffineTransform(point, CGAffineTransformInvert(_transform));
}

- (CGPoint)locationInDocumentWithTouch:(UITouch *)touch
{
    return [self locationInDocument:[touch locationInView:self]];
}

#pragma mark GestureRecognizer

- (void)panGestureRecognizerAction:(UIPanGestureRecognizer *)sender
{
    if (UIGestureRecognizerStateBegan == sender.state) {
        _last.velocity = CGPointZero;
        _last.transform = _transform;
        _panning = YES;
    }
    
    CGPoint translate = [sender translationInView:self];
    CGAffineTransform t = CGAffineTransformMakeTranslation(translate.x, translate.y);
    _transform = CGAffineTransformConcat(_last.transform, t);
    
    if (UIGestureRecognizerStateEnded == sender.state) {
        _last.velocity = [sender velocityInView:self];
        _panning = NO;
    }
    
    [self setNeedsDisplay];
}

- (void)pinchGestureRecognizerAction:(UIPinchGestureRecognizer *)sender
{
    if (UIGestureRecognizerStateBegan == sender.state) {
        _last.velocity = CGPointZero;
        _last.transform = _transform;
        _last.point = [sender locationInView:self];
        _pinching = YES;
    }
    
    if (1 < sender.numberOfTouches) {
        CGPoint point = [sender locationInView:self];
        CGPoint translate = {point.x - _last.point.x, point.y - _last.point.y};
        
        CGAffineTransform t = CGAffineTransformIdentity;
        t = CGAffineTransformTranslate(t, point.x, point.y);
        t = CGAffineTransformScale(t, sender.scale, sender.scale);
        t = CGAffineTransformTranslate(t, -point.x, -point.y);
        t = CGAffineTransformTranslate(t, translate.x, translate.y);
        
        _transform = CGAffineTransformConcat(_last.transform, t);
        _last.pinchCenter = point;
    }
    
    if (UIGestureRecognizerStateEnded == sender.state) {
        _pinching = NO;
    }
    
    [self setNeedsDisplay];
}

#pragma mark DocumentObserver

- (void)document:(Document *)document notify:(va_list)argList
{
    Document::Event event = static_cast<Document::Event>(va_arg(argList, int));
    
    switch (event) {
        case Document::Event::Thickness:
        case Document::Event::Color:
        case Document::Event::BackgroundColor:
        case Document::Event::BackgroundEnable:
        case Document::Event::Paths:
        case Document::Event::PaintPaths:
            [self setNeedsDisplay];
            break;
        case Document::Event::PreprocessedImage:
            if (_drawPreprocessedImage) {
                [self setNeedsDisplay];
            }
            break;
        default:
            break;
    }
}

@end
