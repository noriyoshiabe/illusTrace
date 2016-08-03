//
//  EditorShapeEraserViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/08/01.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditorShapeEraserViewController.h"
#import "EditorObserver.h"
#import "Color.h"

using namespace illustrace;

@interface EditorShapeEraserViewController () <EditorObserver, PreviewViewDelegate> {
    EditorObserverBridge _editorObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *radiusSlider;
@property (weak, nonatomic) IBOutlet UIButton *moveButton;
@property (assign, nonatomic) BOOL move;
@end

@implementation EditorShapeEraserViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _radiusSlider.minimumValue = 5.0;
    _radiusSlider.maximumValue = 50.0;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    _editor->addObserver(&_editorObserverBridge);
    
    _previewView.drawPreprocessedImage = YES;
    self.move = NO;
    
    [self update];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->removeObserver(&_editorObserverBridge);
    _previewView.delegate = nil;
    _previewView.drawPreprocessedImage = NO;
}

- (void)update
{
    _radiusSlider.value = _editor->radius();
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

- (IBAction)radiusSliderAction:(id)sender
{
    _editor->radius(_radiusSlider.value);
}

- (IBAction)moveButtonAction:(id)sender
{
    self.move = !_move;
}

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::PreprocessedImageRadius:
            [self update];
            break;
        default:
            break;
    }
}

#pragma mark PreviewViewDelegate

- (void)previewView:(PreviewView *)previewView touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    __Trace__
}

- (void)previewView:(PreviewView *)previewView touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    __Trace__
}

- (void)previewView:(PreviewView *)previewView touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    __Trace__
}

- (void)previewView:(PreviewView *)previewView touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    __Trace__
}

@end
