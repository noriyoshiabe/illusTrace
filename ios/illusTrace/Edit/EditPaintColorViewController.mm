//
//  EditPaintColorViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/25.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditPaintColorViewController.h"
#import "EditorObserver.h"

using namespace illustrace;

@interface EditPaintColorViewController () <EditorObserver> {
    EditorObserverBridge _editorObserverBridge;
}

@property (weak, nonatomic) IBOutlet UISlider *redSlider;
@property (weak, nonatomic) IBOutlet UISlider *greenSlider;
@property (weak, nonatomic) IBOutlet UISlider *blueSlider;

@end

@implementation EditPaintColorViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorObserverBridge.observer = self;
    
    _redSlider.minimumValue = 0.0;
    _redSlider.maximumValue = 1.0;
    _greenSlider.minimumValue = 0.0;
    _greenSlider.maximumValue = 1.0;
    _blueSlider.minimumValue = 0.0;
    _blueSlider.maximumValue = 1.0;
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
    
    _previewView.scrollEnabled = YES;
    _previewView.zoomEnabled = YES;
    
    [self update];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    _editor->removeObserver(&_editorObserverBridge);
}

- (void)update
{
    auto &color = _editor->paintColor();
    
    _redSlider.value = color[0] / 255.0;
    _greenSlider.value = color[1] / 255.0;
    _blueSlider.value = color[2] / 255.0;
}

- (IBAction)redSliderAction:(id)sender
{
    _editor->R(_redSlider.value);
}

- (IBAction)greenSliderAction:(id)sender
{
    _editor->G(_greenSlider.value);
}

- (IBAction)blueSliderAction:(id)sender
{
    _editor->B(_blueSlider.value);
}

#pragma mark EditorObserver

- (void)editor:(illustrace::Editor *)editor notify:(va_list)argList
{
    Editor::Event event = static_cast<Editor::Event>(va_arg(argList, int));
    
    switch (event) {
        case Editor::Event::PaintColor:
            [self update];
            break;
        default:
            break;
    }
}

@end
