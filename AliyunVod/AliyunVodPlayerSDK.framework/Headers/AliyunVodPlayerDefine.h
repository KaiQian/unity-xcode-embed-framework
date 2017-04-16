//
//  AliyunVodPlayerDefine.h
//  AliyunVodPlayerSDK
//
//  Created by SMY on 16/8/31.
//  Copyright © 2016年 SMY. All rights reserved.
//

static const NSString *AL_DELEGATE_KEY_VIDEO = @"video";
static const NSString *AL_DELEGATE_KEY_ERROR_CODE = @"error_code";
static const NSString *AL_DELEGATE_KEY_SERVER_ERROR_CODE = @"server_error_code";
static const NSString *AL_DELEGATE_KEY_ERROR_MESSAGE = @"error_message";
static const NSString *AL_DELEGATE_KEY_SPEED = @"speed";
static const NSString *AL_DELEGATE_KEY_TIME = @"time";

typedef NS_ENUM(int, AliyunVodPlayerVideoQuality) { //视频质量
    AliyunVodPlayerVideoSmooth,        // 流畅
    AliyunVodPlayerVideoSD,            // 标清
    AliyunVodPlayerVideoHD,            // 高清
    AliyunVodPlayerVideoHD2,           // 超清
    AliyunVodPlayerVideo2K,            // 2K
    AliyunVodPlayerVideo4K,            // 4K
    AliyunVodPlayerVideoRAW,           // 原始
};

typedef NS_ENUM(int, AliyunVodPlayerEvent) {       //视频事件回馈
    AliyunVodPlayerEventError,
    AliyunVodPlayerEventPrepareDone,
    AliyunVodPlayerEventPlay,
    AliyunVodPlayerEventPause,
    AliyunVodPlayerEventStop,
    AliyunVodPlayerEventNetworkSpeed,
    AliyunVodPlayerEventLoadedTime,
    AliyunVodPlayerEventCurrentTime,
    AliyunVodPlayerEventBeginLoading,
    AliyunVodPlayerEventEndLoading,
    AliyunVodPlayerEventWillSwitchToQuality,
    AliyunVodPlayerEventDidSwitchToQuality,
    AliyunVodPlayerEventFailSwitchToQuality,
    AliyunVodPlayerEventSeekDone
};

typedef NS_ENUM(int, AliyunVodPlayerDisplayMode) {  //浏览方式
    AliyunVodPlayerDisplayModeFit,            // 保持原始比例
    AliyunVodPlayerDisplayModeFitWithCropping // 全屏占满屏幕
};

typedef NS_ENUM(int, AliyunVodPlayerState) {  //视频状态
    AliyunVodPlayerStateIdle,        //空转，闲时，静态
    AliyunVodPlayerStateError,       //错误
    AliyunVodPlayerStatePreparing,   //准备中
    AliyunVodPlayerStatePrepared,    //已准备好
    AliyunVodPlayerStatePlay,        //播放
    AliyunVodPlayerStatePause,       //暂停
    AliyunVodPlayerStateStop,        //停止
    AliyunVodPlayerStateLoading    //加载中
};

typedef NS_ENUM(int, AliyunVodPlayerDecodeScheme) { //解码方式
    AliyunVodPlayerDecodeSchemeSoftware,      // 软解，支持很多格式例如 mp4, flv, avi, rmvb, mov ...
    AliyunVodPlayerDecodeSchemeHardwareGPU,   // 硬解
    AliyunVodPlayerDecodeSchemeHardwareYouku, // 仅支持优酷站内同步的视频
    AliyunVodPlayerDecodeSchemeAVFoundation,  // 仅支持本地缓存视频mp4, mov和Apple's HLS stream
};
