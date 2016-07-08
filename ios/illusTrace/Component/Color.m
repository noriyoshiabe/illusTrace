//
//  Color.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "Color.h"

static UIColor *_systemBlueColor;

@implementation Color

+ (void)initialize
{
    _systemBlueColor = [UIColor colorWithRed:0.0 green:.478431373 blue:1.0 alpha:1.0];
}

+ (UIColor *)systemBlueColor
{
    return _systemBlueColor;
}

@end
