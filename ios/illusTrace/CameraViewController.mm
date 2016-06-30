//
//  CameraViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/06/30.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "CameraViewController.h"
#import <opencv2/videoio/cap_ios.h>
#import "Filter.h"

#define kCvPreviewWidth 480.0
#define kCvPreviewHeight 640.0

using namespace illustrace;

@interface CameraViewController () <CvVideoCameraDelegate> {
    CvVideoCamera* _videoCamera;
}
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
@end

@implementation CameraViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _videoCamera = [[CvVideoCamera alloc] initWithParentView:_imageView];
    _videoCamera.delegate = self;
    _videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;
    _videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset640x480;
    _videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    _videoCamera.defaultFPS = 30;
    _videoCamera.grayscaleMode = YES;
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

#pragma mark - CvVideoCameraDelegate

- (void)processImage:(cv::Mat &)image
{
    Filter::threshold(image);
}

@end
