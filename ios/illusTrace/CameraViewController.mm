//
//  CameraViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/06/30.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "CameraViewController.h"
#import <opencv2/videoio/cap_ios.h>
#import "Illustrace.h"

#define kCvPreviewWidth 480.0
#define kCvPreviewHeight 640.0

using namespace illustrace;

@interface CameraViewController () <CvVideoCameraDelegate> {
    CvVideoCamera* _videoCamera;
    Document *_document;
    Illustrace _illustrace;
    CGColorSpaceRef _colorSpace;
    CGContextRef _bitmapContext;
}
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
@end

@implementation CameraViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _document = new Document();
    
    _videoCamera = [[CvVideoCamera alloc] initWithParentView:_imageView];
    _videoCamera.delegate = self;
    _videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;
    _videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset640x480;
    _videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    _videoCamera.defaultFPS = 30;
    
    _colorSpace = CGColorSpaceCreateDeviceRGB();
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [_videoCamera start];
        
        CALayer *layer = [_imageView.layer.sublayers objectAtIndex:0];
        
        CGFloat ratiox = kCvPreviewWidth / layer.frame.size.width;
        CGFloat ratioy = kCvPreviewHeight / layer.frame.size.height;
        CGFloat ratio = (ratiox < ratioy)? ratiox : ratioy;
        CGFloat w = kCvPreviewWidth / ratio;
        CGFloat h = kCvPreviewHeight / ratio;
        CGFloat x = (layer.frame.size.width - w) / 2.0;
        CGFloat y = (layer.frame.size.height - h) / 2.0;
        
        layer.frame = CGRectMake(x, y, w, h);
    });
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewWillDisappear:true];
    [_videoCamera stop];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)drawPaths
{
    CGContextSetRGBStrokeColor(_bitmapContext, 0.0, 0.0, 1.0, 0.5);
    CGContextSetRGBFillColor(_bitmapContext, 0.0, 0.0, 1.0, 0.5);
    CGContextSetLineWidth(_bitmapContext, _document->thickness());
    CGContextSetLineCap(_bitmapContext, kCGLineCapRound);
    
    for (auto *path : *_document->paths()) {
        CGMutablePathRef pathRef = CGPathCreateMutable();
        
        [self drawPath:path subPath:pathRef];
        
        if (path->closed) {
            CGContextAddPath(_bitmapContext, pathRef);
            CGContextEOFillPath(_bitmapContext);
        }
        
        CGContextAddPath(_bitmapContext, pathRef);
        CGContextStrokePath(_bitmapContext);
        
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

#pragma mark - CvVideoCameraDelegate

- (void)processImage:(cv::Mat &)image
{
    _illustrace.traceForPreview(image, _document);
    
    if (!_bitmapContext) {
        _bitmapContext = CGBitmapContextCreate(image.data, image.cols, image.rows, 8, image.cols * 4, _colorSpace, kCGImageAlphaPremultipliedLast);
        CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, image.rows);
        CGContextConcatCTM(_bitmapContext, flipVertical);
    }
    
    [self drawPaths];
}

@end