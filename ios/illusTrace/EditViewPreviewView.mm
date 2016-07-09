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
    CGFloat _identityScale;
    
    CGAffineTransform _transform;
    
    struct {
        CGAffineTransform transform;
        CGPoint point;
        CGPoint velocity;
        CFAbsoluteTime time;
    } _last;
    
    bool _panning;
    bool _pinching;
}
@end

@implementation EditViewPreviewView

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    UIPanGestureRecognizer *panGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGestureRecognizerAction:)];
    [self addGestureRecognizer:panGestureRecognizer];
    
    UIPinchGestureRecognizer *pinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchGestureRecognizerAction:)];
    [self addGestureRecognizer:pinchGestureRecognizer];
    
    _transform = CGAffineTransformIdentity;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    auto contentRect = _document->contentRect();
    CGFloat contentAspect = (CGFloat)contentRect.height / (CGFloat)contentRect.width;
    CGFloat viewAspect = self.bounds.size.height / self.bounds.size.width;
    
    CGFloat scale = contentAspect < viewAspect ? self.bounds.size.height / contentRect.height : self.bounds.size.width / contentRect.width;
    CGAffineTransform scaleT = CGAffineTransformMakeScale(scale, scale);
    CGAffineTransform translationT = CGAffineTransformMakeTranslation((self.bounds.size.width - contentRect.width * scale) / 2.0, (self.bounds.size.height - contentRect.height * scale) / 2.0);
    
    _identity = CGAffineTransformConcat(scaleT, translationT);
    _identityScale = 1.0 / scale;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextConcatCTM(context, _identity);
    CGContextConcatCTM(context, _transform);
    
    [self drawPaths:context];
    
    CGContextClipToRect(context, rect);
    
    CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
    [self inertia:time - _last.time];
    _last.time = time;
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

- (void)inertia:(CFAbsoluteTime)delta
{
    bool needsDisplay = NO;
    
    if (1 <= _last.velocity.x || 1 <= _last.velocity.y) {
        CGFloat f = (CGFloat)delta / 1.0;
        
        CGAffineTransform t = CGAffineTransformMakeTranslation(_last.velocity.x * f, _last.velocity.y * f);
        _transform = CGAffineTransformConcat(_transform, t);
        
        _last.velocity.x *= 0.95;
        _last.velocity.y *= 0.95;
        
        needsDisplay = YES;
    }
    
    if (!_pinching) {
        if (1.0 > _transform.a) {
            if (0.99 < _transform.a) {
                _transform = CGAffineTransformIdentity;
            }
            else {
                _transform.a = CGAffineTransformIdentity.a + (_transform.a - CGAffineTransformIdentity.a) * 0.9;
                _transform.b = CGAffineTransformIdentity.b + (_transform.b - CGAffineTransformIdentity.b) * 0.9;
                _transform.c = CGAffineTransformIdentity.c + (_transform.c - CGAffineTransformIdentity.c) * 0.9;
                _transform.d = CGAffineTransformIdentity.d + (_transform.d - CGAffineTransformIdentity.d) * 0.9;
                _transform.tx = CGAffineTransformIdentity.tx + (_transform.tx - CGAffineTransformIdentity.tx) * 0.9;
                _transform.ty = CGAffineTransformIdentity.ty + (_transform.ty - CGAffineTransformIdentity.ty) * 0.9;
            }
            
            needsDisplay = YES;
        }
        else if (3.0 < _transform.a) {
            if (3.01 > _transform.a) {
                _transform.a = 3.0;
                _transform.d = 3.0;
            }
            else {
                CGAffineTransform t = CGAffineTransformIdentity;
                
                CGSize size = self.bounds.size;
                t = CGAffineTransformTranslate(t, size.width / 2.0 * _identityScale, size.height / 2.0 * _identityScale);
                t = CGAffineTransformScale(t, 0.9, 0.9);
                t = CGAffineTransformTranslate(t, -size.width / 2.0 * _identityScale, -size.height / 2.0 * _identityScale);
                
                _transform = CGAffineTransformConcat(_transform, t);
            }
            
            needsDisplay = YES;
        }
        
    }
    
    if (needsDisplay) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setNeedsDisplay];
        });
    }
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
    
    CGAffineTransform t = CGAffineTransformIdentity;
    t = CGAffineTransformTranslate(t, translate.x * _identityScale, translate.y * _identityScale);
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
        
        t = CGAffineTransformTranslate(t, point.x * _identityScale, point.y * _identityScale);
        t = CGAffineTransformScale(t, sender.scale, sender.scale);
        t = CGAffineTransformTranslate(t, -point.x * _identityScale, -point.y * _identityScale);
        
        t = CGAffineTransformTranslate(t, translate.x * _identityScale, translate.y * _identityScale);
        
        _transform = CGAffineTransformConcat(_last.transform, t);
    }
    
    if (UIGestureRecognizerStateEnded == sender.state) {
        _pinching = NO;
    }
    
    [self setNeedsDisplay];
}

@end
