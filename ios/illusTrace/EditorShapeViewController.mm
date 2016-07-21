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

@interface EditorShapeViewController () <DocumentObserver, EditorObserver> {
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
    _editor->addObserver(&_editorObserverBridge);
    _editor->document->addObserver(&_documentObserverBridge);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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

#pragma mark DocumentObserver

- (void)document:(illustrace::Document *)document notify:(va_list)argList
{
    Document::Event event = static_cast<Document::Event>(va_arg(argList, int));
    
    switch (event) {
        case Document::Event::Detail:
            _detailSlider.value = document->detail();
            break;
        case Document::Event::Thickness:
            _thicknessSlider.value = document->thickness();
            break;
        default:
            break;
    }
}

#pragma mark EditorObserver

- (void)editor:(Editor *)editor notify:(va_list)argList
{
    // TODO
}

@end
