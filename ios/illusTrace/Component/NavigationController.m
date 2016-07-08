//
//  NavigationController.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "NavigationController.h"

@interface NavigationController ()

@end

@implementation NavigationController

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item
{
    if (!_navigationBarDelegate || [_navigationBarDelegate navigationBar:navigationBar shouldPopItem:item]) {
        [self popViewControllerAnimated:YES];
        return YES;
    }
    
    for (UIView *subview in navigationBar.subviews) {
        if(0. < subview.alpha && subview.alpha < 1.) {
            [UIView animateWithDuration:.25 animations:^{
                subview.alpha = 1.;
            }];
        }
    }
    
    return NO;
}

@end
