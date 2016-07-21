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

@interface EditViewController () <PreviewViewDelegate, EditorObserver> {
    Illustrace _illustrace;
    Editor *_editor;
    
    EditorObserverBridge _editorObserverBridge;
}
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
    
    _editor = new Editor(&_illustrace, _document);
    _editor->addObserver(&_editorObserverBridge);
    _editorObserverBridge.observer = self;
    
    _shapeVC = [EditorShapeViewController new];
    _shapeVC.editor = _editor;
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

- (void)update
{
    // TODO;
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

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    // TODO
}

@end
