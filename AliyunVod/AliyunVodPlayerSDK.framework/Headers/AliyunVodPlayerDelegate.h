//
//  AliyunVodPlayerDelegate.h
//  AliyunVodPlayerSDK
//
//  Created by SMY on 16/7/10.
//  Copyright © 2016年 SMY. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AliyunVodPlayerDefine.h"

@protocol AliyunVodPlayerDelegate <NSObject>

@required
- (void)alPlayerOnEventCallback:(AliyunVodPlayerEvent)event params:(NSDictionary *)params;

@end
