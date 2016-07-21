//
//  DocumentObserver.h
//  illusTrace
//
//  Created by abechan on 2016/07/22.
//  Copyright © 2016年 Noriyoshi Abe. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Document.h"

@protocol DocumentObserver <NSObject>
- (void)document:(illustrace::Document *)document notify:(va_list)argList;
@end

class DocumentObserverBridge : public illustrace::Observer <illustrace::Document> {
public:
    void notify(illustrace::Document *sender, va_list argList) {
        [this->observer document:sender notify:argList];
    }
    
    id <DocumentObserver> observer;
};
