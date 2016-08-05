//
//  EditPaintViewController.h
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Editor.h"
#import "PreviewView.h"

@interface EditPaintViewController : UIViewController
@property (assign, nonatomic) illustrace::Editor *editor;
@property (strong, nonatomic) PreviewView *previewView;
@end
