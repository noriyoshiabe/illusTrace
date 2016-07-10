//
//  DocumentViewController.mm
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "DocumentViewController.h"
#import "PreviewView.h"
#import "NavigationController.h"

using namespace illustrace;

@interface DocumentViewController () <UINavigationBarDelegate>
@property (weak, nonatomic) IBOutlet PreviewView *previewView;
@end

@implementation DocumentViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    ((NavigationController *)self.navigationController).navigationBarDelegate = self;
    
    _previewView.document = _document;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item
{
    return YES;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
