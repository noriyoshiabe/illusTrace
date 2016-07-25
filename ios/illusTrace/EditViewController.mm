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
#import "Illustrace.h"
#import "Editor.h"
#import "EditorObderver.h"

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
@property (weak, nonatomic) IBOutlet UIBarButtonItem *backgroundButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *brushButtonItem;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *trimmingButtonItem;
@property (weak, nonatomic) IBOutlet UIView *paletContainer;
@property (weak, nonatomic) UIViewController *activePaletVC;
@property (strong, nonatomic) EditorShapeViewController *shapeVC;
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
    
    _shapeVC = [EditorShapeViewController new];
    _shapeVC.editor = _editor;
    _shapeVC.previewView = _previewView;
    
    self.activePaletVC = _shapeVC;
    
    [self update];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setActivePaletVC:(UIViewController *)activePaletVC
{
    activePaletVC.view.frame = _paletContainer.bounds;
    
    [_activePaletVC viewWillDisappear:NO];
    [activePaletVC viewWillAppear:NO];
    
    [_activePaletVC.view removeFromSuperview];
    [_paletContainer addSubview:activePaletVC.view];
    
    [_activePaletVC viewDidDisappear:NO];
    [activePaletVC viewDidAppear:NO];
    
    _activePaletVC = activePaletVC;
}

- (void)update
{
    _undoButtonItem.enabled = _editor->canUndo();
    _redoButtonItem.enabled = _editor->canRedo();
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
            // TODO
            break;
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
