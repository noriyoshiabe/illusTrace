//
//  EditViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/11.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "EditViewController.h"
#import "PreviewView.h"

@interface EditViewController ()
@end

@implementation EditViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.preview.document = _document;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (PreviewView *)preview
{
    return (PreviewView *)self.view;
}

@end
