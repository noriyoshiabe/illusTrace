//
//  EditorShapeViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditorShapeViewController.h"
#import "DocumentObserver.h"
#import "EditorObderver.h"

using namespace illustrace;

@interface EditorShapeViewController () <DocumentObserver, EditorObserver, PreviewViewDelegate> {
    DocumentObserverBridge _documentObserverBridge;
    EditorObserverBridge _editorObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *detailSlider;
@property (weak, nonatomic) IBOutlet UISlider *thicknessSlider;
@end

@implementation EditorShapeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    _documentObserverBridge.observer = self;
    
    _detailSlider.maximumValue = 1.0;
    _detailSlider.minimumValue = 0.1;
    
    _thicknessSlider.minimumValue = 1.0;
    _thicknessSlider.maximumValue = 50.0;
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
    _editor->document->addObserver(&_documentObserverBridge);
    _editor->lineState(Editor::LineState::Line);
    
    _previewView.delegate = self;
    
    [self update];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->removeObserver(&_editorObserverBridge);
    _editor->document->removeObserver(&_documentObserverBridge);
    
    _previewView.delegate = nil;
}

- (void)update
{
    _detailSlider.value = _editor->document->detail();
    _thicknessSlider.value = _editor->document->thickness();
}

#pragma mark Tool actions

- (IBAction)pathAction:(id)sender
{
    __Trace__
}

- (IBAction)colorAction:(id)sender
{
    __Trace__
}

- (IBAction)pencilAction:(id)sender
{
    __Trace__
}

- (IBAction)eraserAction:(id)sender
{
    __Trace__
}

- (IBAction)reloadAction:(id)sender
{
    __Trace__
}

- (IBAction)detailSliderAction:(id)sender
{
    _editor->detail(_detailSlider.value);
}

- (IBAction)thicknessSliderAction:(id)sender
{
    _editor->thickness(_thicknessSlider.value);
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

#pragma mark DocumentObserver

- (void)document:(illustrace::Document *)document notify:(va_list)argList
{
    Document::Event event = static_cast<Document::Event>(va_arg(argList, int));
    
    switch (event) {
        case Document::Event::Detail:
        case Document::Event::Thickness:
            [self update];
            break;
        default:
            break;
    }
}

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::LineState:
            switch (_editor->lineState()) {
                case Editor::LineState::Line:
                    _previewView.scrollEnabled = YES;
                    _previewView.zoomEnabled = YES;
                    _previewView.touchCallbackEnabled = NO;
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        default:
            break;
    }
}

@end
