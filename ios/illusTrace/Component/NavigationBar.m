//
//  NavigationBar.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "NavigationBar.h"

@implementation NavigationBar

- (void)awakeFromNib
{
    [super awakeFromNib];
    [self setBackgroundImage:[UIImage new] forBarMetrics:UIBarMetricsDefault];
    self.shadowImage = [UIImage new];
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *v = [super hitTest:point withEvent:event];
    if (v != self) {
        return v;
    }
    else {
        return 80.0 > point.x ? v : nil;
    }
}

@end
