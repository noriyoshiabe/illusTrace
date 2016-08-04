//
//  EditShapeLineViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditShapeLineViewController.h"
#import "DocumentObserver.h"

using namespace illustrace;

@interface EditShapeLineViewController () <DocumentObserver> {
    DocumentObserverBridge _documentObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *detailSlider;
@property (weak, nonatomic) IBOutlet UISlider *thicknessSlider;
@end

@implementation EditShapeLineViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
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
    
    _editor->document->addObserver(&_documentObserverBridge);
    
    _previewView.scrollEnabled = YES;
    _previewView.zoomEnabled = YES;
    
    [self update];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->document->removeObserver(&_documentObserverBridge);
}

- (void)update
{
    _detailSlider.value = _editor->document->detail();
    _thicknessSlider.value = _editor->document->thickness();
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
        case Document::Event::Thickness:
            [self update];
            break;
        default:
            break;
    }
}

@end
