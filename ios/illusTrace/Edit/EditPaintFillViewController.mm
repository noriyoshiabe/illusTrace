//
//  EditPaintFillViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/08/01.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditPaintFillViewController.h"
#import "Color.h"

using namespace illustrace;

@interface EditPaintFillViewController () <PreviewViewDelegate>
@property (weak, nonatomic) IBOutlet UIButton *moveButton;
@property (assign, nonatomic) BOOL move;
@end

@implementation EditPaintFillViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    _previewView.drawPaintLayer = YES;
    self.move = NO;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    _previewView.delegate = nil;
    _previewView.drawPaintLayer = NO;
}

- (void)setMove:(BOOL)move
{
    if (move) {
        _previewView.scrollEnabled = YES;
        _previewView.zoomEnabled = YES;
        _previewView.delegate = nil;
        _moveButton.tintColor = [Color systemBlueColor];
    }
    else {
        _previewView.scrollEnabled = NO;
        _previewView.zoomEnabled = NO;
        _previewView.delegate = self;
        _moveButton.tintColor = [UIColor darkGrayColor];
    }
    
    _move = move;
}

- (IBAction)moveButtonAction:(id)sender
{
    self.move = !_move;
}

#pragma mark PreviewViewDelegate

- (void)previewView:(PreviewView *)previewView touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    CGPoint point = [previewView locationInDocumentWithTouch:touches.anyObject];
    _editor->fill(point.x, point.y);
}

- (void)previewView:(PreviewView *)previewView touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
}

- (void)previewView:(PreviewView *)previewView touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
}

- (void)previewView:(PreviewView *)previewView touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
}

@end
