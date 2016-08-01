//
//  EditorObserver.h
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Editor.h"

@protocol EditorObserver <NSObject>
- (void)editor:(illustrace::Editor *)editor notify:(va_list)argList;
@end

class EditorObserverBridge : public illustrace::Observer <illustrace::Editor> {
public:
    void notify(illustrace::Editor *sender, va_list argList) {
        [this->observer editor:sender notify:argList];
    }
    
    id <EditorObserver> observer;
};