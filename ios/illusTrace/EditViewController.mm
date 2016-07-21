//
//  EditViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/11.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditViewController.h"
#import "PreviewView.h"
#import "EditorShapeViewController.h"

@interface EditViewController () <PreviewViewDelegate>
@property (weak, nonatomic) IBOutlet PreviewView *previewView;
@property (weak, nonatomic) IBOutlet UIToolbar *toolbar;
@property (strong, nonatomic) EditorShapeViewController *shapeVC;
@property (weak, nonatomic) IBOutlet UIView *paletContainer;
@end

@implementation EditViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    _previewView.document = _document;
    _previewView.delegate = self;
    
    _previewView.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"tile"]];
    
    _shapeVC = [EditorShapeViewController new];
    _shapeVC.view.frame = _paletContainer.bounds;
    [_paletContainer addSubview:_shapeVC.view];
    
//    _previewView.scrollEnabled = NO;
//    _previewView.zoomEnabled = NO;
//    _previewView.touchCallbackEnabled = YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark Toolbar actions

- (IBAction)undoAction:(id)sender
{
    __Trace__
}

- (IBAction)redoAction:(id)sender
{
    __Trace__
}

- (IBAction)shapeAction:(id)sender
{
    __Trace__
}

- (IBAction)backgroundAction:(id)sender
{
    __Trace__
}

- (IBAction)brushAction:(id)sender
{
    __Trace__
}

- (IBAction)trimmingAction:(id)sender
{
    __Trace__
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
