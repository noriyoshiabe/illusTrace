//
//  EditViewPreviewView.h
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Document.h"

@interface EditViewPreviewView : UIView
@property (assign, nonatomic) illustrace::Document *document;
@end
