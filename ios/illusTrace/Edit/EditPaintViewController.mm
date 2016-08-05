//
//  EditPaintViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditPaintViewController.h"
#import "EditPaintColorViewController.h"
#import "EditPaintBrushViewController.h"
#import "EditPaintFillViewController.h"
#import "EditorObserver.h"
#import "Color.h"

using namespace illustrace;

@interface EditPaintViewController () <EditorObserver> {
    EditorObserverBridge _editorObserverBridge;
}
@property (weak, nonatomic) IBOutlet UIButton *colorButton;
@property (weak, nonatomic) IBOutlet UIButton *brushButton;
@property (weak, nonatomic) IBOutlet UIButton *fillButton;
@property (weak, nonatomic) IBOutlet UIButton *eraserButton;
@property (weak, nonatomic) IBOutlet UIView *childContainer;
@property (weak, nonatomic) UIViewController *activeVC;
@property (strong, nonatomic) EditPaintColorViewController *colorVC;
@property (strong, nonatomic) EditPaintBrushViewController *brushVC;
@property (strong, nonatomic) EditPaintFillViewController *fillVC;
@property (strong, nonatomic) EditPaintBrushViewController *eraserVC;
@end

@implementation EditPaintViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _colorVC = [EditPaintColorViewController new];
    _colorVC.editor = _editor;
    _colorVC.previewView = _previewView;
    
    _brushVC = [EditPaintBrushViewController new];
    _brushVC.editor = _editor;
    _brushVC.previewView = _previewView;
    
    _fillVC = [EditPaintFillViewController new];
    _fillVC.editor = _editor;
    _fillVC.previewView = _previewView;
    
    _eraserVC = [EditPaintBrushViewController new];
    _eraserVC.editor = _editor;
    _eraserVC.previewView = _previewView;
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
    _editor->paintState(Editor::PaintState::Color);
    
    self.activeVC = _colorVC;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->removeObserver(&_editorObserverBridge);
}

- (void)setActiveVC:(UIViewController *)activeVC
{
    if (_activeVC == activeVC) {
        return;
    }
    
    activeVC.view.frame = _childContainer.bounds;
    
    [_activeVC viewWillDisappear:NO];
    [activeVC viewWillAppear:NO];
    
    [_activeVC.view removeFromSuperview];
    [_childContainer addSubview:activeVC.view];
    
    [_activeVC viewDidDisappear:NO];
    [activeVC viewDidAppear:NO];
    
    _activeVC = activeVC;
}

- (void)update
{
    _colorButton.tintColor = nil;
    _brushButton.tintColor = nil;
    _fillButton.tintColor = nil;
    _eraserButton.tintColor = nil;
    
    switch (_editor->paintState()) {
        case Editor::PaintState::Color:
            _colorButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::PaintState::Brush:
            _brushButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::PaintState::Fill:
            _fillButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::PaintState::Eraser:
             _eraserButton.tintColor = [Color systemBlueColor];
            break;
    }
}

#pragma mark Tool actions

- (IBAction)colorAction:(id)sender
{
    _editor->paintState(Editor::PaintState::Color);
    self.activeVC = _colorVC;
}

- (IBAction)brushAction:(id)sender
{
    _editor->paintState(Editor::PaintState::Brush);
    self.activeVC = _brushVC;
}

- (IBAction)fillAction:(id)sender
{
    _editor->paintState(Editor::PaintState::Fill);
    self.activeVC = _fillVC;
}

- (IBAction)eraserAction:(id)sender
{
    _editor->paintState(Editor::PaintState::Eraser);
    self.activeVC = _eraserVC;
}

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::PaintState:
            [self update];
            break;
        default:
            break;
    }
}

@end
