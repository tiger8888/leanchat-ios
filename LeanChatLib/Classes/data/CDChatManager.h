//
//  CDChatManager.h
//  LeanChat
//
//  Created by lzw on 15/1/21.
//  Copyright (c) 2015年 AVOS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CDUserModel.h"
#import "AVIMConversation+Custom.h"

static NSString *const kCDNotificationMessageReceived = @"MessageReceived";
static NSString *const kCDNotificationMessageDelivered = @"MessageDelivered";
static NSString *const kCDNotificationConversationUpdated = @"ConversationUpdated";
static NSString *const kCDNotificationConnectivityUpdated = @"ConnectStatus";

typedef void (^CDRecentConversationsCallback)(NSArray *conversations, NSInteger totalUnreadCount,  NSError *error);

@protocol CDUserDelegate <NSObject>

@required

//同步方法
- (id <CDUserModel> )getUserById:(NSString *)userId;

//对于每条消息，都会调用这个方法来缓存发送者的用户信息，以便 getUserById 直接返回用户信息
- (void)cacheUserByIds:(NSSet *)userIds block:(AVBooleanResultBlock)block;

@end

@interface CDChatManager : NSObject

@property (nonatomic, strong) id <CDUserDelegate> userDelegate;

@property (nonatomic, strong, readonly) NSString *selfId;
@property (nonatomic, strong) id <CDUserModel> selfUser;
@property (nonatomic, assign, readonly) BOOL connect;

+ (instancetype)manager;

- (AVIMClient *)imClient;

- (void)openWithClientId:(NSString *)clientId callback:(AVIMBooleanResultBlock)callback;
- (void)closeWithCallback:(AVBooleanResultBlock)callback;

- (void)fecthConvWithConvid:(NSString *)convid callback:(AVIMConversationResultBlock)callback;
- (void)fetchConvsWithConvids:(NSSet *)convids callback:(AVIMArrayResultBlock)callback;
- (void)fetchConvWithOtherId:(NSString *)otherId callback:(AVIMConversationResultBlock)callback;
- (void)fetchConvWithMembers:(NSArray *)members callback:(AVIMConversationResultBlock)callback;
- (void)findGroupedConvsWithBlock:(AVIMArrayResultBlock)block;

- (void)createConvWithMembers:(NSArray *)members type:(CDConvType)type callback:(AVIMConversationResultBlock)callback;
- (void)updateConv:(AVIMConversation *)conv name:(NSString *)name attrs:(NSDictionary *)attrs callback:(AVIMBooleanResultBlock)callback;

- (void)queryTypedMessagesWithConversation:(AVIMConversation *)conversation timestamp:(int64_t)timestamp limit:(NSInteger)limit block:(AVIMArrayResultBlock)block;

- (void)findRecentConversationsWithBlock:(CDRecentConversationsCallback)block;
- (void)setZeroUnreadWithConversationId:(NSString *)conversationId;
- (void)deleteUnreadByConversationId:(NSString *)conversationId;
- (void)incrementUnreadWithConversationId:(NSString *)conversationId;

- (NSString *)getPathByObjectId:(NSString *)objectId;
- (NSString *)tmpPath;
- (NSString *)uuid;

@end
