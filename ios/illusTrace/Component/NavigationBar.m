//
//  NavigationBar.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "NavigationBar.h"

@implementation NavigationBar

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)awakeFromNib
{
    [super awakeFromNib];
    [self setBackgroundImage:[UIImage new] forBarMetrics:UIBarMetricsDefault];
    self.shadowImage = [UIImage new];
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *v = [super hitTest:point withEvent:event];
    return v == self ? nil : v;
}

@end
