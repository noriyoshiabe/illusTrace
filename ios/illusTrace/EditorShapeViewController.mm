//
//  EditorShapeViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditorShapeViewController.h"
#import "DocumentObserver.h"

using namespace illustrace;

@interface EditorShapeViewController () <DocumentObserver> {
    DocumentObserverBridge documentObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *detailSlider;
@property (weak, nonatomic) IBOutlet UISlider *thicknessSlider;
@end

@implementation EditorShapeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    documentObserverBridge.observer = self;
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
    __Trace__
}

- (IBAction)thicknessSliderAction:(id)sender
{
    __Trace__
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

@end
