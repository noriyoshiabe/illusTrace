//
//  CameraViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/06/30.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "CameraViewController.h"
#import <opencv2/videoio/cap_ios.h>

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
    
}

@end
