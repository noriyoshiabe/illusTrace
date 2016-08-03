//
//  PreviewView.h
//  illusTrace
//
//  Created by abechan on 2016/07/09.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Document.h"

@class PreviewView;
@protocol PreviewViewDelegate <NSObject>
- (void)previewView:(PreviewView *)previewView touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)previewView:(PreviewView *)previewView touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)previewView:(PreviewView *)previewView touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)previewView:(PreviewView *)previewView touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
@end

@interface PreviewView : UIView
@property (assign, nonatomic) id<PreviewViewDelegate> delegate;
@property (assign, nonatomic) illustrace::Document *document;
@property (assign, nonatomic) BOOL scrollEnabled;
@property (assign, nonatomic) BOOL zoomEnabled;
@property (assign, nonatomic) BOOL drawPreprocessedImage;
- (CGPoint)locationInDocument:(CGPoint)point;
- (CGPoint)locationInDocumentWithTouch:(UITouch *)touch;
@end
