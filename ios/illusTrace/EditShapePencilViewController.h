//
//  EditShapePencilViewController.h
//  illusTrace
//
//  Created by abechan on 2016/08/01.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Editor.h"
#import "PreviewView.h"

@interface EditShapePencilViewController : UIViewController
@property (assign, nonatomic) illustrace::Editor *editor;
@property (strong, nonatomic) PreviewView *previewView;
@end
