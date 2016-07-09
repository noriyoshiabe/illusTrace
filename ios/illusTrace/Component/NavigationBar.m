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
    
    CGRect rect = CGRectMake(0, 0, 1, 1);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [UIColor colorWithWhite:1.0 alpha:0.2].CGColor);
    CGContextFillRect(context, rect);
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    [self setBackgroundImage:image forBarMetrics:UIBarMetricsDefault];
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
