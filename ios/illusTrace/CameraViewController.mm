//
//  CameraViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/06/30.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "CameraViewController.h"
#import "Illustrace.h"
#import "Color.h"
#import "EditViewController.h"

#import <AVFoundation/AVFoundation.h>

using namespace illustrace;

@interface CameraViewController () <AVCaptureVideoDataOutputSampleBufferDelegate> {
    Illustrace _illustrace;
    AVCaptureDevice *_camera;
    AVCaptureDeviceInput *_videoInput;
    AVCaptureStillImageOutput *_stillImageOutput;
    AVCaptureVideoDataOutput *_videoDataOutput;
    AVCaptureSession *_session;
    BOOL _lightEnable;
    BOOL _negative;
    CGFloat _zoomScale;
    
    Document *_document;
}
@property (weak, nonatomic) IBOutlet UIImageView *previewView;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *lightButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *shapeButton;
@property (weak, nonatomic) IBOutlet UISlider *brightnessSlider;
@end

@implementation CameraViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSError *error = nil;
    _session = [[AVCaptureSession alloc] init];
    _session.sessionPreset = AVCaptureSessionPresetPhoto;
    
    _camera = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    
    _videoInput = [[AVCaptureDeviceInput alloc] initWithDevice:_camera error:&error];
    [_session addInput:_videoInput];
    
    _videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    [_session addOutput:_videoDataOutput];
    
    dispatch_queue_t queue = dispatch_queue_create("captureQueue", NULL);
    _videoDataOutput.alwaysDiscardsLateVideoFrames = YES;
    _videoDataOutput.videoSettings = @{(NSString *)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA)};
    [_videoDataOutput setSampleBufferDelegate:self queue:queue];
    
    _stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
    _stillImageOutput.outputSettings = @{(NSString *)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange)};
    [_session addOutput:_stillImageOutput];
    
    UIPinchGestureRecognizer *pinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchGestureRecognizerAction:)];
    [_previewView addGestureRecognizer:pinchGestureRecognizer];
    
    UITapGestureRecognizer *doubleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapGestureRecognizerAction:)];
    doubleTapRecognizer.numberOfTapsRequired = 2;
    [_previewView addGestureRecognizer:doubleTapRecognizer];
    
    _previewView.userInteractionEnabled = YES;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [_session startRunning];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [_session stopRunning];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)infoButtonAction:(id)sender
{
}

- (IBAction)lightButtonAction:(id)sender
{
    _lightEnable = !_lightEnable;
    _lightButton.tintColor = _lightEnable ? [Color systemBlueColor] : nil;
    
    if ([_camera lockForConfiguration:nil]) {
        [_camera setTorchMode:_lightEnable ? AVCaptureTorchModeOn : AVCaptureTorchModeOff];
        [_camera unlockForConfiguration];
    }
}

- (IBAction)shapeButtonAction:(id)sender
{
    _negative = !_negative;
    _shapeButton.image = [UIImage imageNamed:_negative ? @"shape_invert" : @"shape"];
}

- (IBAction)takeButtonAction:(id)sender
{
    AVCaptureConnection *videoConnection = [_stillImageOutput connectionWithMediaType:AVMediaTypeVideo];
    if (videoConnection) {
        [_stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
             if (imageDataSampleBuffer) {
                 CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(imageDataSampleBuffer);
                 CVPixelBufferLockBaseAddress(pixelBuffer, 0);
                 
                 uint8_t *grayImage = (uint8_t *)CVPixelBufferGetBaseAddress(pixelBuffer);
                 size_t width = CVPixelBufferGetWidth(pixelBuffer);
                 size_t height = CVPixelBufferGetHeight(pixelBuffer);
                 cv::Mat sourceImage((int)height, (int)width, CV_8U, grayImage, width);
                 
                 int resizedCols = round(1000.0 * ((CGFloat)width / (CGFloat)height));
                 cv::Mat resizedImage = cv::Mat::ones(1000, resizedCols, CV_8U);
                 cv::resize(sourceImage, resizedImage, resizedImage.size(), cv::INTER_CUBIC);
                 cv::transpose(resizedImage, resizedImage);
                 cv::flip(resizedImage, resizedImage, 1);
                 
                 CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
                 
                 _document = new Document();
                 _document->brightness(_brightnessSlider.value);
                 _document->negative(_negative);
                 
                 _illustrace.traceFromImage(resizedImage, _document);
                 
                 [self performSegueWithIdentifier:@"Edit" sender:self];
             }
         }];
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"Edit"]) {
        EditViewController *vc = segue.destinationViewController;
        vc.document = _document;
        _document = NULL;
    }
}

#pragma mark AVCaptureVideoDataOutputSampleBufferDelegate

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer);
    size_t width = CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    
    cv::Mat sourceImage((int)height, (int)width, CV_8UC4, baseAddress, bytesPerRow);
    std::vector<std::vector<cv::Point>> outlineContours;
    std::vector<cv::Vec4i> outlineHierarchy;
    
    _illustrace.traceForPreview(sourceImage, outlineContours, outlineHierarchy, _brightnessSlider.value, _negative);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef bitmapContext = CGBitmapContextCreate(baseAddress, width, height, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    
    CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, height);
    CGContextConcatCTM(bitmapContext, flipVertical);
    
    UIColor *color = [Color systemBlueColor];
    CGFloat r, g, b;
    [color getRed:&r green:&g blue:&b alpha:NULL];
    
    CGContextSetRGBStrokeColor(bitmapContext, r, g, b, 1.0);
    CGContextSetRGBFillColor(bitmapContext, r, g, b, 0.2);

    CGContextSetLineWidth(bitmapContext, 1.0);
    CGContextSetLineCap(bitmapContext, kCGLineCapRound);
    
    CGMutablePathRef pathRef = CGPathCreateMutable();
    
    for (auto contour : outlineContours) {
        CGPathMoveToPoint(pathRef, NULL, contour[0].x, contour[0].y);
        
        size_t length = contour.size();
        for (int i = 1; i < length; ++i) {
            CGPathAddLineToPoint(pathRef, NULL, contour[i].x, contour[i].y);
        }
    }
    
    CGContextAddPath(bitmapContext, pathRef);
    CGContextEOFillPath(bitmapContext);
    
    CGContextAddPath(bitmapContext, pathRef);
    CGContextStrokePath(bitmapContext);
    
    CGPathRelease(pathRef);
    
    CGImageRef cgImage = CGBitmapContextCreateImage(bitmapContext);
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    CGContextRelease(bitmapContext);
    CGColorSpaceRelease(colorSpace);
    
    UIImage *image = [UIImage imageWithCGImage:cgImage scale:1.0 orientation:UIImageOrientationRight];
    CGImageRelease(cgImage);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        _previewView.image = image;
    });
}

#pragma mark GestureRecognizer

- (void)pinchGestureRecognizerAction:(UIPinchGestureRecognizer *)sender
{
    if (UIGestureRecognizerStateBegan == sender.state) {
        _zoomScale = _camera.videoZoomFactor;
    }
    
    if (UIGestureRecognizerStateChanged == sender.state) {
        if ([_camera lockForConfiguration:nil]) {
            _camera.videoZoomFactor = MAX(1.0, MIN(_zoomScale * sender.scale, _camera.activeFormat.videoMaxZoomFactor));
            [_camera unlockForConfiguration];
        }
    }
}

- (void)doubleTapGestureRecognizerAction:(UITapGestureRecognizer *)sender
{
    if ([_camera lockForConfiguration:nil]) {
        _camera.videoZoomFactor = 1.0;
        [_camera unlockForConfiguration];
    }
}

@end
