//
//  Slider.m
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import "Slider.h"

@implementation Slider

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
    [self setThumbImage:[UIImage imageNamed:@"slider_thumb"] forState:UIControlStateNormal];
}

- (CGRect)trackRectForBounds:(CGRect)bounds
{
    CGRect _bounds = [super trackRectForBounds:bounds];
    _bounds.size.height = 1;
    return _bounds;
}

@end
