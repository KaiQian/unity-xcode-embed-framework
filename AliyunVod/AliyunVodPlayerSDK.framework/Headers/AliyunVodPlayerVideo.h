//
//  AliyunVodPlayerVideo.h
//  AliyunVodPlayerSDK
//
//  Created by SMY on 16/7/6.
//  Copyright © 2016年 SMY. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AliyunVodPlayerDefine.h"

/*
 AliyunVodPlayerVideo为播放视频信息，prepareDone时候返回该类信息
 */
@interface AliyunVodPlayerVideo : NSObject {
    
}

/*
 videoId为视频ID
 */
@property (nonatomic, strong) NSString *videoId;

/*
 title为视频显示的标题
 */
@property (nonatomic, strong) NSString *title;

/*
 duration为视频总时长，单位为秒
 */
@property (nonatomic, assign) double duration; //秒

/*
 allSupportQuality为所有支持的清晰度，返回一个清晰度数组
 */
- (NSArray *)allSupportQuality;

@end
