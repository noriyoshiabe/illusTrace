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
    uchar *_imageData;
    
    AVCaptureDevice *_videoDevice;
    CGFloat _zoomScale;
}
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
@property (weak, nonatomic) IBOutlet UISlider *brightnessSlider;
@property (weak, nonatomic) IBOutlet UISlider *detailSlider;
@property (weak, nonatomic) IBOutlet UISlider *smoothingSlider;
@property (weak, nonatomic) IBOutlet UISlider *thicknessSlider;
@property (weak, nonatomic) IBOutlet UISegmentedControl *modeControl;
@property (weak, nonatomic) IBOutlet UISegmentedControl *colorControl;
@property (weak, nonatomic) IBOutlet UISwitch *lightSwitch;
@end

@implementation CameraViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _document = new Document();
    _document->mode(LineMode::Outline);
    
    _videoCamera = [[CvVideoCamera alloc] initWithParentView:_imageView];
    _videoCamera.delegate = self;
    _videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;
    _videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset640x480;
    _videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    _videoCamera.defaultFPS = 30;
    
    _colorSpace = CGColorSpaceCreateDeviceRGB();
    
    _imageView.userInteractionEnabled = YES;
    
    UIPinchGestureRecognizer *pinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchGestureRecognizerAction:)];
    [_imageView addGestureRecognizer:pinchGestureRecognizer];
    
    UITapGestureRecognizer *singleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(singleTapGestureRecognizerAction:)];
    singleTapRecognizer.numberOfTapsRequired = 1;
    [_imageView addGestureRecognizer:singleTapRecognizer];
    UITapGestureRecognizer *doubleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapGestureRecognizerAction:)];
    doubleTapRecognizer.numberOfTapsRequired = 2;
    [_imageView addGestureRecognizer:doubleTapRecognizer];
    [singleTapRecognizer requireGestureRecognizerToFail:doubleTapRecognizer];
    
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices) {
        if (AVCaptureDevicePositionBack == [device position]) {
            _videoDevice = device;
            
            if ([_videoDevice lockForConfiguration:nil]) {
                _videoDevice.focusMode = AVCaptureFocusModeAutoFocus;
                _videoDevice.exposureMode = AVCaptureExposureModeAutoExpose;
                [_videoDevice unlockForConfiguration];
            }
            
            _lightSwitch.enabled = _videoDevice.hasTorch;
        }
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [_videoCamera start];
        
        CALayer *layer = [_imageView.layer.sublayers objectAtIndex:0];
        
        CGFloat ratiox = kCvPreviewWidth / layer.frame.size.width;
        CGFloat ratioy = kCvPreviewHeight / layer.frame.size.height;
        CGFloat ratio = (ratiox > ratioy)? ratiox : ratioy;
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
    if (1 == _colorControl.selectedSegmentIndex) {
        CGContextSetRGBFillColor(_bitmapContext, 1.0, 1.0, 1.0, 1.0);
        CGContextFillRect(_bitmapContext, (CGRect){CGPointZero, {kCvPreviewWidth, kCvPreviewHeight}});
        
        CGContextSetRGBStrokeColor(_bitmapContext, 0.0, 0.0, 0.0, 1.0);
        CGContextSetRGBFillColor(_bitmapContext, 0.0, 0.0, 0.0, 1.0);
    }
    else {
        CGContextSetRGBStrokeColor(_bitmapContext, 0.0, 0.0, 1.0, 0.5);
        CGContextSetRGBFillColor(_bitmapContext, 0.0, 0.0, 1.0, 0.5);
    }
    
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
    
    if (_imageData != image.data) {
        if (_bitmapContext) {
            CGContextRelease(_bitmapContext);
            _bitmapContext = NULL;
        }
        
        _imageData = image.data;
    }
    
    if (!_bitmapContext) {
        _bitmapContext = CGBitmapContextCreate(image.data, image.cols, image.rows, 8, image.cols * 4, _colorSpace, kCGImageAlphaPremultipliedLast);
        CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, image.rows);
        CGContextConcatCTM(_bitmapContext, flipVertical);
    }
    
    [self drawPaths];
}

#pragma mark IBAction

- (IBAction)brightnessSliderAction:(UISlider *)sender
{
    _document->brightness(sender.value);
}

- (IBAction)detailSliderAction:(UISlider *)sender
{
    _document->detail(MAX(0.1, sender.value));
}

- (IBAction)smoothingSliderAction:(UISlider *)sender
{
    _document->smoothing(sender.value);
}

- (IBAction)thicknessSliderAction:(UISlider *)sender
{
    _document->thickness(sender.value);
}

- (IBAction)modeControllSliderAction:(UISegmentedControl *)sender
{
    switch (sender.selectedSegmentIndex) {
        case 0:
            _document->mode(LineMode::Outline);
            break;
        case 1:
            _document->mode(LineMode::Center);
            break;
    }
}

- (IBAction)lightSwitchAction:(UISwitch *)sender
{
    if ([_videoDevice lockForConfiguration:nil]) {
        [_videoDevice setTorchMode:sender.isOn ? AVCaptureTorchModeOn : AVCaptureTorchModeOff];
        _videoDevice.focusMode = AVCaptureFocusModeAutoFocus;
        _videoDevice.exposureMode = AVCaptureExposureModeAutoExpose;
        [_videoDevice unlockForConfiguration];
    }
}

#pragma mark GestureRecognizer

- (void)pinchGestureRecognizerAction:(UIPinchGestureRecognizer *)sender
{
    if (UIGestureRecognizerStateBegan == sender.state) {
        _zoomScale = _videoDevice.videoZoomFactor;
    }
    
    if (UIGestureRecognizerStateChanged == sender.state) {
        if ([_videoDevice lockForConfiguration:nil]) {
            _videoDevice.videoZoomFactor = MAX(1.0, MIN(_zoomScale * sender.scale, _videoDevice.activeFormat.videoMaxZoomFactor));
            [_videoDevice unlockForConfiguration];
        }
    }
}

- (void)singleTapGestureRecognizerAction:(UITapGestureRecognizer *)sender
{
    if ([_videoDevice lockForConfiguration:nil]) {
        CGPoint point = [sender locationInView: _imageView];
        point.x /= _imageView.frame.size.width;
        point.y /= _imageView.frame.size.height;
        
        _videoDevice.focusPointOfInterest = point;
        _videoDevice.focusMode = AVCaptureFocusModeAutoFocus;
        _videoDevice.exposurePointOfInterest = point;
        _videoDevice.exposureMode = AVCaptureExposureModeAutoExpose;
        [_videoDevice unlockForConfiguration];
    }
}

- (void)doubleTapGestureRecognizerAction:(UITapGestureRecognizer *)sender
{
    if ([_videoDevice lockForConfiguration:nil]) {
        _videoDevice.videoZoomFactor = 1.0;
        [_videoDevice unlockForConfiguration];
    }
}

@end
