//
//  AliyunVodPlayer.h
//  AliyunVodPlayerSDK
//
//  Created by SMY on 16/6/28.
//  Copyright © 2016年 SMY. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import "AliyunVodPlayerDefine.h"
#import "AliyunVodPlayerDelegate.h"

/*
 AliyunVodPlayer为播放器类，实现播放器的各种控制
 */
@interface AliyunVodPlayer : NSObject

/*
 功能：设置用户的AK，和ApiKey
 参数：
    acessKeyID：用户AK ID
    accessKeySecret：用户AK Secret
    apiKey:API key，在控制台获取得到
 备注：必须要设置，否则无法进行播放，具体获取方式请参考文档，这几个值必须要进行必要保护，否则会被泄露，建议把这几个值存放到你的服务器上，然后动态更新
 */
- (void) initAccessKeyAndApiKey:(NSString *)acessKeyID andAppSecret:(NSString *)accessKeySecret andApiKey:(NSString*)apiKey;

/*
 功能：播放器初始化视频，主要目的是分析视频内容，读取视频头信息，解析视频流中的视频和音频信息，并根据视频和音频信息去寻找解码器，创建播放线程等
 参数：vid,输入视频的video id
 备注：调用该函数完成后立即返回，需要等待准备完成通知，收到该通知后代表视频初始化完成，视频准备完成后可以获取到视频的相关信息。
 */
- (void)prepareWithVid:(NSString *)vid;

/*
 功能：开始播放视频
 备注：在prepareWithVid之后可以调用start进行播放。
 */
- (void)start;

/*
 功能：恢复播放视频
 备注：在pause暂停视频之后可以调用resume进行播放。
 */
- (void)resume;

/*
 功能：暂停播放视频
 备注：在start播放视频之后可以调用pause进行暂停。
 */
- (void)pause;

/*
 功能：停止播放视频
 */
- (void)stop;

/*
 功能：跳转到指定位置进行播放，单位为秒
 备注：在开始播放之后才能调用此函数。
 */
- (void)seekTo:(NSTimeInterval)time;

/*
 功能：视频总长度，单位为秒
 备注：在prepareWithVid之后才能够获取时长。
 */
- (NSTimeInterval)duration;

/*
 功能：当前视频播放位置，单位为秒
 备注：在开始播放之后才能够获取当前播放位置。
 */
- (NSTimeInterval)currentTime;

/*
 功能：获取已经加载的视频长度，或者说对于网络视频来说已经下载的视频时长
 备注：在开始播放之后才能够获取此位置。
 */
- (NSTimeInterval)loadedTime;

/*
 功能：设置播放器代理，播放器的各种通知通过代理进行反馈 
 
 - (void)alPlayerOnEventCallback:(AliyunVodPlayerEvent)event params:(NSDictionary *)params;

 代理的事件信息有:
 
 事件枚举                             事件说明              param中的key值       param中的value值
 AliyunVodPlayerEventError                 播放器准备异常         error_code          错误码：int
 AliyunVodPlayerEventPrepareDone           播放器准备完毕         video               当前播放的视频对象：AliyunVodPlayerVideo
 AliyunVodPlayerEventPlay                  播放器开始播放         无                   无
 AliyunVodPlayerEventPause                 播放器暂停播放         无                    无
 AliyunVodPlayerEventStop                  播放器停止播放         无                    无
 AliyunVodPlayerEventNetworkSpeed          播放器当前网络速度      speed                当前网络速度（注：只对非HLS视频有效）
 AliyunVodPlayerEventLoadedTime            播放器预加载时间        time                预加载的时间（单位：秒）
 AliyunVodPlayerEventCurrentTime           播放器当前播放时间      time                 当前播放的时间（单位：秒）
 AliyunVodPlayerEventBeginLoading          播放器开始加载          无                  无
 AliyunVodPlayerEventEndLoading            播放器结束加载         无                   无
 AliyunVodPlayerEventWillSwitchToQuality	播放器将要切换清晰度	quality             要切换的清晰度
 AliyunVodPlayerEventDidSwitchToQuality     播放器切换清晰度完成	quality             要切换的清晰度
 AliyunVodPlayerEventFailSwitchToQuality	播放器切换清晰度失败	quality             要切换的清晰度

 事件的参数通过一个NSDictionary的param进行反馈，具体的反馈信息可以参考demo中的代码
 */
- (void)setAliyunVodPlayerDelegate:(id<AliyunVodPlayerDelegate>)delegate;

/*
 功能：获取视频显示的窗口，
 备注：此窗口为内部创建的窗口，用户可以用来添加到用户界面上进行显示
 */
@property (nonatomic, assign, readonly) UIView *playerView;

/*
 功能：获取/设置显示模式
 显示模式有：
 AliyunVodPlayerDisplayModeFit,            // 保持原始比例
 AliyunVodPlayerDisplayModeFitWithCropping // 全屏占满屏幕
 */
@property (nonatomic) AliyunVodPlayerDisplayMode displayMode;

/*
 功能：设置/获取是否自动播放
 备注：如果设置成自动播放，则prepareWithVid之后不需要调用start进行播放
 */
@property (nonatomic, getter=isAutoPlay) BOOL autoPlay;

/*
 功能：设置/获取清晰度，用来切换视频清晰度
 播放器的清晰度有：
 AliyunVodPlayerVideoSmooth,        // 流畅
 AliyunVodPlayerVideoSD,            // 标清
 AliyunVodPlayerVideoHD,            // 高清
 AliyunVodPlayerVideoHD2,           // 超清
 AliyunVodPlayerVideo2K,            // 2K
 AliyunVodPlayerVideo4K,            // 4K
 AliyunVodPlayerVideoRAW,           // 原始
 
 备注：在播放之后才能调用
 */
@property (nonatomic) AliyunVodPlayerVideoQuality videoQuality;

/*
 功能：获取播放器当前播放状态
 当前播放状态有：
 AliyunVodPlayerStateIdle,        //空转，闲时，静态
 AliyunVodPlayerStateError,       //错误
 AliyunVodPlayerStatePreparing,   //准备中
 AliyunVodPlayerStatePrepared,    //已准备好
 AliyunVodPlayerStatePlay,        //播放
 AliyunVodPlayerStatePause,       //暂停
 AliyunVodPlayerStateStop,        //停止
 AliyunVodPlayerStateLoading      //加载中
 */
- (AliyunVodPlayerState) playerState;

/*
 功能：获取此播放器版本号
 */
- (NSString*) getSDKVersion;

@end
