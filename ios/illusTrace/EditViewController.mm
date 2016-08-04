//
//  EditViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/11.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditViewController.h"
#import "PreviewView.h"
#import "EditShapeViewController.h"
#import "Illustrace.h"
#import "Editor.h"
#import "EditorObserver.h"
#import "Color.h"

using namespace illustrace;

@interface EditViewController () <EditorObserver> {
    Illustrace _illustrace;
    Editor *_editor;
    
    EditorObserverBridge _editorObserverBridge;
}
@property (weak, nonatomic) IBOutlet PreviewView *previewView;
@property (weak, nonatomic) IBOutlet UIToolbar *toolbar;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *undoButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *redoButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *shapeButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *backgroundButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *brushButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *trimmingButtonItem;
@property (weak, nonatomic) IBOutlet UIView *childContainer;
@property (weak, nonatomic) UIViewController *activeVC;
@property (strong, nonatomic) EditShapeViewController *shapeVC;
@end

@implementation EditViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    _previewView.document = _document;
    _previewView.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"tile"]];
    
    _editor = new Editor(&_illustrace, _document);
    _editor->addObserver(&_editorObserverBridge);
    _editorObserverBridge.observer = self;
    
    _shapeVC = [EditShapeViewController new];
    _shapeVC.editor = _editor;
    _shapeVC.previewView = _previewView;
    
    _editor->mode(Editor::Mode::Shape);
    self.activeVC = _shapeVC;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
    _undoButtonItem.enabled = _editor->canUndo();
    _redoButtonItem.enabled = _editor->canRedo();
    
    _shapeButtonItem.tintColor = nil;
    _backgroundButtonItem.tintColor = nil;
    _brushButtonItem.tintColor = nil;
    _trimmingButtonItem.tintColor = nil;
    
    switch (_editor->mode()) {
        case Editor::Mode::Shape:
            _shapeButtonItem.tintColor = [Color systemBlueColor];
            break;
        case Editor::Mode::BG:
            _backgroundButtonItem.tintColor = [Color systemBlueColor];
            break;
        case Editor::Mode::Paint:
            _brushButtonItem.tintColor = [Color systemBlueColor];
            break;
        case Editor::Mode::Clip:
            _trimmingButtonItem.tintColor = [Color systemBlueColor];
            break;
    }
}

#pragma mark Toolbar actions

- (IBAction)undoAction:(id)sender
{
    _editor->undo();
}

- (IBAction)redoAction:(id)sender
{
    _editor->redo();
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

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));

    switch (event) {
        case Editor::Event::Mode:
        case Editor::Event::Execute:
        case Editor::Event::Undo:
        case Editor::Event::Redo:
            [self update];
            break;
        case Editor::Event::Save:
            // TODO;
            break;
        default:
            break;
    }
}

@end
