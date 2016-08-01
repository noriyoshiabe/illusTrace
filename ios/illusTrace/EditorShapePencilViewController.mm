//
//  EditorShapePencilViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/08/01.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditorShapePencilViewController.h"
#import "EditorObserver.h"

using namespace illustrace;

@interface EditorShapePencilViewController () <EditorObserver, PreviewViewDelegate> {
    EditorObserverBridge _editorObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *radiusSlider;

@end

@implementation EditorShapePencilViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _radiusSlider.minimumValue = 0.0;
    _radiusSlider.maximumValue = 1.0;
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
    
    _previewView.scrollEnabled = YES;
    _previewView.zoomEnabled = YES;
    _previewView.delegate = self;
    _previewView.drawPreprocessedImage = YES;
    
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
