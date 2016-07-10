//
//  DocumentViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "DocumentViewController.h"
#import "PreviewView.h"
#import "EditViewController.h"

using namespace illustrace;

@interface DocumentViewController ()
@property (strong, nonatomic) EditViewController *editorViewController;
@property (strong, nonatomic) IBOutlet UIBarButtonItem *retakeButton;
@end

@implementation DocumentViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _editorViewController = [EditViewController new];
    _editorViewController.document = _document;
    _editorViewController.view.frame = self.view.bounds;
    [self.view addSubview:_editorViewController.view];
    
    self.navigationItem.hidesBackButton = YES;
    self.navigationItem.leftBarButtonItems = @[_retakeButton];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)retakeButtonAction:(id)sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

@end
