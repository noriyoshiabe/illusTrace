//
//  Toolbar.m
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "Toolbar.h"

@implementation Toolbar

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    [self setBackgroundImage:[UIImage new] forToolbarPosition:UIBarPositionAny barMetrics:UIBarMetricsDefault];
    [self setShadowImage:[UIImage new] forToolbarPosition:UIBarPositionAny];
}

@end
