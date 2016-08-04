//
//  EditShapeViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditShapeViewController.h"
#import "EditShapeLineViewController.h"
#import "EditShapeColorViewController.h"
#import "EditShapePencilViewController.h"
#import "EditorObserver.h"
#import "Color.h"

using namespace illustrace;

@interface EditShapeViewController () <EditorObserver> {
    EditorObserverBridge _editorObserverBridge;
}
@property (weak, nonatomic) IBOutlet UIButton *lineButton;
@property (weak, nonatomic) IBOutlet UIButton *colorButton;
@property (weak, nonatomic) IBOutlet UIButton *pencilButton;
@property (weak, nonatomic) IBOutlet UIButton *eraserButton;
@property (weak, nonatomic) IBOutlet UIButton *reloadButton;
@property (weak, nonatomic) IBOutlet UIView *childContainer;
@property (weak, nonatomic) UIViewController *activeVC;
@property (strong, nonatomic) EditShapeLineViewController *lineVC;
@property (strong, nonatomic) EditShapeColorViewController *colorVC;
@property (strong, nonatomic) EditShapePencilViewController *pencilVC;
@property (strong, nonatomic) EditShapePencilViewController *eraserVC;
@end

@implementation EditShapeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _lineVC = [EditShapeLineViewController new];
    _lineVC.editor = _editor;
    _lineVC.previewView = _previewView;
    
    _colorVC = [EditShapeColorViewController new];
    _colorVC.editor = _editor;
    _colorVC.previewView = _previewView;
    
    _pencilVC = [EditShapePencilViewController new];
    _pencilVC.editor = _editor;
    _pencilVC.previewView = _previewView;
    
    _eraserVC = [EditShapePencilViewController new];
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
    _editor->shapeState(Editor::ShapeState::Line);
    
    self.activeVC = _lineVC;
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
    _lineButton.tintColor = nil;
    _colorButton.tintColor = nil;
    _pencilButton.tintColor = nil;
    _eraserButton.tintColor = nil;
    
    switch (_editor->shapeState()) {
        case Editor::ShapeState::Line:
            _lineButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::ShapeState::Pencil:
            _pencilButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::ShapeState::Eraser:
             _eraserButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::ShapeState::Color:
             _colorButton.tintColor = [Color systemBlueColor];
            break;
    }
}

#pragma mark Tool actions

- (IBAction)lineAction:(id)sender
{
    _editor->shapeState(Editor::ShapeState::Line);
    self.activeVC = _lineVC;
}

- (IBAction)colorAction:(id)sender
{
    _editor->shapeState(Editor::ShapeState::Color);
    self.activeVC = _colorVC;
}

- (IBAction)pencilAction:(id)sender
{
    _editor->shapeState(Editor::ShapeState::Pencil);
    self.activeVC = _pencilVC;
}

- (IBAction)eraserAction:(id)sender
{
    _editor->shapeState(Editor::ShapeState::Eraser);
    self.activeVC = _eraserVC;
}

- (IBAction)reloadAction:(id)sender
{
    _editor->reload();
}

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::ShapeState:
            [self update];
            break;
        default:
            break;
    }
}

@end
