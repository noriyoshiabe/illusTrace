//
//  NavigationController.h
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NavigationController : UINavigationController
@property (weak, nonatomic) id<UINavigationBarDelegate> navigationBarDelegate;
@end
