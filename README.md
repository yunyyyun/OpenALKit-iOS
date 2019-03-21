# OpenALKit-iOS

[![CI Status](https://img.shields.io/travis/yunyyyun/OpenALKit-iOS.svg?style=flat)](https://travis-ci.org/yunyyyun/OpenALKit-iOS)
[![Version](https://img.shields.io/cocoapods/v/OpenALKit-iOS.svg?style=flat)](https://cocoapods.org/pods/OpenALKit-iOS)
[![License](https://img.shields.io/cocoapods/l/OpenALKit-iOS.svg?style=flat)](https://cocoapods.org/pods/OpenALKit-iOS)
[![Platform](https://img.shields.io/cocoapods/p/OpenALKit-iOS.svg?style=flat)](https://cocoapods.org/pods/OpenALKit-iOS)

## Example

没有例子

## 介绍

在iOS里使用OpenAL播放声音的一种方式，如果你觉得AVAudioPlayer和SystemSound都不好用，辣么OpenAL可能是你的最佳选择。

## 安装

以下是一个 Podfile 的例子

```ruby
source  'https://github.com/CocoaPods/Specs.git'
source  'https://github.com/yunyyyun/OpenALKit-iOS-Specs.git'
...
pod 'OpenALKit-iOS'
```

##使用

使用步骤：

- 添加将要播放的音源（mp3格式，其他格式也行，不过需要改代码）
- 你也可以修改宏（不过这会修改我的源码），设置音源个数（iOS 支持的最大音源数为 32）,添加数组索引：

```
#define MAX_BUFFER_COUNT        4. //OpenALPlayer.m (11)
gSourceFile = [[NSArray alloc] initWithObjects:
                   @"flyup",@"hit",@"gg",@"start",nil];  //OpenALPlayer.m (129)
```

- 播放：

```
[[OpenALPlayer shared] doPlayWithTag:tag];
```

demo例子简陋，更多声效设置可见OpenALPlayer.m，谢谢！

## Author

yunyyyun, mengyun2012@163.com

## License

OpenALKit-iOS is available under the MIT license. See the LICENSE file for more info.
