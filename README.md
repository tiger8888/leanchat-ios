![leanchat](https://cloud.githubusercontent.com/assets/5022872/7810175/5edda692-03d2-11e5-875d-d32ebfc887a6.gif)


## App Store  
LeanChat 已经在 App Store 上架，可前往 https://itunes.apple.com/gb/app/leanchat/id943324553 或在 App Store 上搜 LeanChat。

## 介绍
这个示例项目全面展示了 LeanCloud 实时通讯功能的应用，但含杂着许多 UI 代码和其它功能，并不适合快速上手学习，如果你第一次接触 LeanMessage，更推荐 [LeanMessage-Demo](https://github.com/leancloud/LeanMessage-Demo) 项目。等熟悉了之后，可前往 [LeanCloud-Demos](https://github.com/leancloud/leancloud-demos) 挑选你喜欢的 IM 皮肤进行集成。集成的过程中，若遇到疑难问题，不妨再来参考 LeanChat 项目。

## 下载
请直接点击 Github 上的`Download Zip`，如图所示，这样只下载最新版。如果是 `git clone`，则可能非常慢，因为含杂很大的提交历史。某次测试两者是1.5M:40M。

![qq20150618-2 2x](https://cloud.githubusercontent.com/assets/5022872/8223520/4c25415a-15ab-11e5-912d-b5dab916ce86.png)

## 运行
```bash
  cd LeanChat
  pod install --verbose --no-repo-update // 拯救短暂的人生
  open LeanChat.workspace
```

这里可以看到三个项目，介绍如下。

## 子项目介绍
* LeanChatLib ，核心的聊天逻辑和聊天界面库。有了它，可以快速集成聊天功能，支持文字、音频、图片、表情消息，消息通知。同时也有相应的 [Android 版本](https://github.com/leancloud/leanchat-android)。
* LeanChatExample，leanchatlib 最简单的使用例子。展示了如何用少量代码调用 LeanChatLib 来加入聊天，无论是用 LeanCloud 的用户系统还是自己的用户系统。
* LeanChat-ios，为 LeanChat 整个应用。它包含好友管理、群组管理、地理消息、附近的人、个人页面、登录注册的功能，完全基于 LeanCloud 的存储和通信功能。它也是对 LeanChatLib 更复杂的应用。

## LeanChatLib 介绍

封装了最近对话页面和聊天页面，LeanChat 和 LeanChatExample 项目都依赖于它。可通过以下方式安装，
```
    pod 'LeanChatLib', '0.1.6'
```

## 如何三步加入IM
1. LeanCloud 中创建应用       
2. 加入 LeanChatLib 的 pod 依赖，或拖动 LeanChatLib 的代码文件进项目，改 UI 和调整功能方便些。
3. 依次在合适的地方加入以下代码，

应用启动后，初始化，以及配置 IM User
```objc
    [AVOSCloud setApplicationId:@"YourAppId" clientKey:@"YourAppKey"];
    [CDChatManager manager].userDelegate = [[CDUserFactory alloc] init];
```

配置一个 UserFactory，遵守 CDUserDelegate协议即可。

```objc
#import "CDUserFactory.h"

#import <LeanChatLib/LeanChatLib.h>

@interface CDUserFactory ()<CDUserDelegate>

@end


@implementation CDUserFactory

#pragma mark - CDUserDelegate
-(void)cacheUserByIds:(NSSet *)userIds block:(AVIMArrayResultBlock)block{
    block(nil,nil); // don't forget it
}

-(id<CDUserModel>)getUserById:(NSString *)userId{
    CDUser* user=[[CDUser alloc] init];
    user.userId=userId;
    user.username=userId;
    user.avatarUrl=@"http://ac-x3o016bx.clouddn.com/86O7RAPx2BtTW5zgZTPGNwH9RZD5vNDtPm1YbIcu";
    return user;
}

@end

```

这里的 CDUser 是应用内的User对象，你可以在你的User对象实现 CDUserModel 协议即可。

CDUserModel，
```objc
@protocol CDUserModel <NSObject>

@required

-(NSString*)userId;

-(NSString*)avatarUrl;

-(NSString*)username;

@end
```

登录时调用，
```objc
        [[CDChatManager manager] openWithClientId:selfId callback: ^(BOOL succeeded, NSError *error) {
            if (error) {
                DLog(@"%@", error);
            }
            else {
               //go Main Controller
            }
        }];
```

和某人聊天，
```objc
        [[CDChatManager manager] fetchConvWithOtherId : otherId callback : ^(AVIMConversation *conversation, NSError *error) {
            if (error) {
                DLog(@"%@", error);
            }
            else {
                LCEChatRoomVC *chatRoomVC = [[LCEChatRoomVC alloc] initWithConv:conversation];
                [weakSelf.navigationController pushViewController:chatRoomVC animated:YES];
            }
        }];
```

和多人群聊，
```objc
        NSMutableArray *memberIds = [NSMutableArray array];
        [memberIds addObject:groupId1];
        [memberIds addObject:groupId2];
        [memberIds addObject:[CDChatManager manager].selfId];
        [[CDChatManager manager] fetchConvWithMembers:memberIds callback: ^(AVIMConversation *conversation, NSError *error) {
            if (error) {
                DLog(@"%@", error);
            }
            else {
                LCEChatRoomVC *chatRoomVC = [[LCEChatRoomVC alloc] initWithConv:conversation];
                [weakSelf.navigationController pushViewController:chatRoomVC animated:YES];
            }
        }];
```

注销时，
```objc
    [[CDChatManager manager] closeWithCallback: ^(BOOL succeeded, NSError *error) {
        
    }];
```

然后，就可以像上面截图那样聊天了。注意，目前我们并不推荐直接用 pod 方式来引入 LeanChatLib ，因为有些界面和功能需要由你来定制，所以推荐将 LeanChatLib 的代码拷贝进项目，这样改起来方便一些。

## 部署 LeanChat 需知

如果要部署完整的LeanChat的话，因为该应用有添加好友的功能，请在设置->应用选项中，勾选互相关注选项，以便一方同意的时候，能互相加好友。

![qq20150407-5](https://cloud.githubusercontent.com/assets/5022872/7016645/53f91bb8-dd1b-11e4-8ce0-72312c655094.png)

## 开发指南

[实时通信服务开发指南](https://leancloud.cn/docs/realtime_v2.html)

[更多介绍](https://github.com/leancloud/leanchat-android)

## 技术支持

关于此项目的问题，欢迎提 [issues](https://github.com/leancloud/leanchat-ios/issues)，或者[搜索工单](https://ticket.leancloud.cn/search?content=LeanChat) 。

## 致谢

感谢曾宪华大神的 [MessageDisplayKit](https://github.com/xhzengAIB/MessageDisplayKit) 开源库。
