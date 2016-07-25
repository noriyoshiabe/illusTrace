//
//  EditorShapeViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditorShapeViewController.h"
#import "EditorShapeLineViewController.h"
#import "EditorObderver.h"
#import "Color.h"

using namespace illustrace;

@interface EditorShapeViewController () <EditorObserver> {
    EditorObserverBridge _editorObserverBridge;
}
@property (weak, nonatomic) IBOutlet UIButton *lineButton;
@property (weak, nonatomic) IBOutlet UIButton *colorButton;
@property (weak, nonatomic) IBOutlet UIButton *pencilButton;
@property (weak, nonatomic) IBOutlet UIButton *eraserButton;
@property (weak, nonatomic) IBOutlet UIButton *reloadButton;
@property (weak, nonatomic) IBOutlet UIView *childContainer;
@property (weak, nonatomic) UIViewController *activeVC;
@property (strong, nonatomic) EditorShapeLineViewController *lineVC;
@end

@implementation EditorShapeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _lineVC = [EditorShapeLineViewController new];
    _lineVC.editor = _editor;
    _lineVC.previewView = _previewView;
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
    _editor->lineState(Editor::LineState::Line);
    
    self.activeVC = _lineVC;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->removeObserver(&_editorObserverBridge);
}

- (void)setActiveVC:(UIViewController *)activeVC
{
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
    
    switch (_editor->lineState()) {
        case Editor::LineState::Line:
            _lineButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::LineState::PencilBlack:
            _pencilButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::LineState::PencilWhite:
            // TODO remove state
            break;
        case Editor::LineState::Eraser:
             _eraserButton.tintColor = [Color systemBlueColor];
            break;
        case Editor::LineState::Color:
             _colorButton.tintColor = [Color systemBlueColor];
            break;
    }
}

#pragma mark Tool actions

- (IBAction)lineAction:(id)sender
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

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::LineState:
            [self update];
            break;
        default:
            break;
    }
}

@end
