//
//  EditViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/11.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditViewController.h"
#import "PreviewView.h"

@interface EditViewController () <PreviewViewDelegate>
@end

@implementation EditViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.preview.document = _document;
    self.preview.delegate = self;
//    self.preview.scrollEnabled = NO;
//    self.preview.zoomEnabled = NO;
//    self.preview.touchCallbackEnabled = YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (PreviewView *)preview
{
    return (PreviewView *)self.view;
}

#pragma mark PreviewViewDelegate

- (void)previewView:(PreviewView *)previewView touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    CGPoint point = [previewView locationInDocument:[touches.anyObject locationInView:previewView]];
    printf("%s %f %f\n", __func__, point.x, point.y);
}

- (void)previewView:(PreviewView *)previewView touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    CGPoint point = [previewView locationInDocument:[touches.anyObject locationInView:previewView]];
    printf("%s %f %f\n", __func__, point.x, point.y);
}

- (void)previewView:(PreviewView *)previewView touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    CGPoint point = [previewView locationInDocument:[touches.anyObject locationInView:previewView]];
    printf("%s %f %f\n", __func__, point.x, point.y);
}

- (void)previewView:(PreviewView *)previewView touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    CGPoint point = [previewView locationInDocument:[touches.anyObject locationInView:previewView]];
    printf("%s %f %f\n", __func__, point.x, point.y);
}

@end
