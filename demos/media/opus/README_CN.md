# Opus说明文档

Opus是一种有损声音编码的格式，详情参考<a href="https://github.com/xiph/opus" target="_blank">opus</a>。

## Opus接口介绍

本文档介绍一些常用接口，更多更详细的接口介绍请参考`components/media/opus/opus-1.3.1/include/opus.h`文件。

Opus解码的过程是将opus格式转换为pcm格式，编码的过程是将pcm格式转换为opus格式。

关于pcm格式的详情请参考<a href="https://baike.baidu.com/item/pcm%E7%BC%96%E7%A0%81/10865033?fr=aladdin" target="_blank">pcm</a>。


### 解码
-  创建解码器
    ```C
    OPUS_EXPORT OPUS_WARN_UNUSED_RESULT OpusDecoder *opus_decoder_create(
        opus_int32 Fs,
        int channels,
        int *error
    );
    ```
-  解码器配置
    ```C
    OPUS_EXPORT int opus_decoder_ctl(OpusDecoder *st, int request, ...) OPUS_ARG_NONNULL(1);
    ```
-  解码
    ```C
    OPUS_EXPORT OPUS_WARN_UNUSED_RESULT int opus_decode(
        OpusDecoder *st,
        const unsigned char *data,
        opus_int32 len,
        opus_int16 *pcm,
        int frame_size,
        int decode_fec
    ) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(4);
    ```
-  删除解码器

    ```C
    OPUS_EXPORT void opus_decoder_destroy(OpusDecoder *st);
    ```
### 编码

-  创建编码器
    ```C
    OPUS_EXPORT OPUS_WARN_UNUSED_RESULT OpusEncoder *opus_encoder_create(
        opus_int32 Fs,
        int channels,
        int application,
        int *error
    );
    ```
-  编码器配置
    ```C
    OPUS_EXPORT int opus_encoder_ctl(OpusEncoder *st, int request, ...) OPUS_ARG_NONNULL(1);
    ```
-  编码
    ```C
    OPUS_EXPORT OPUS_WARN_UNUSED_RESULT opus_int32 opus_encode(
        OpusEncoder *st,
        const opus_int16 *pcm,
        int frame_size,
        unsigned char *data,
        opus_int32 max_data_bytes
    ) OPUS_ARG_NONNULL(1) OPUS_ARG_NONNULL(2) OPUS_ARG_NONNULL(4);
    ```
-  删除编码器
    ```C
    OPUS_EXPORT void opus_encoder_destroy(OpusEncoder *st);
    ```

### Opus编解码中常用参数

- sampling rate
采样率，在opus中只能取值为8000，12000，16000，24000，48000，单位为赫兹（HZ）。
- channel
声道，channel = 1为单声道，channel = 2为立体声。
- frame size
帧大小，在opus中每帧的时间取值为2.5，5，10，20，40，60，80，100，120。单位毫秒（ms）。
单通道的帧大小 = 采样率 * 帧时间（单位为秒）。

其他详情参考<a href="https://baike.baidu.com/item/%E9%9F%B3%E9%A2%91%E7%BC%96%E7%A0%81/1729208?fr=aladdin" target="_blank">音频编码</a>。

## Opus Demo解析

LiteOS中提供了`OpusDemoTask`程序来演示如何使用`opus`。该Demo可以运行在LiteOS已支持的所有开发板上。

本Demo以解码为例，其中`inputBuf`为opus格式编码的一段数据，解码出的数据以pcm格式保存到`outputBuf`。

### 使能Opus Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Opus Demo。
    ```
    Demos  --->
        Media Demo  --->
                [*] Enable Opus Demo
    ```
使能Opus Demo后会自动使能Opus组件。

保存退出后，LiteOS会从github上自动下载opus源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

### 编译运行Opus Demo
使能Opus Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`libopus.a`和`libopus_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

以realview-pbx-a9为例，运行`OpusDemoTask`。

其他开发板操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

执行结果如下

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
Processor  : Cortex-A9 * 4
Run Mode   : SMP
GIC Rev    : GICv1
build time : Mar 12 2021 22:52:50

**********************************

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 2 entering scheduler
cpu 3 entering scheduler
cpu 1 entering scheduler
app init!
Hello, welcome to liteos demo!
Opus demo task start to run.
Opus demo input data:
0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0xe0 
Opus demo output data:
0x0000 0xfffd 0xfff4 0xffda 0xffaa 0xff88 0xff55 0xff03 0xff14 0xff83 0xffb4 0x001f 0x0070 0x01bb 0x024b 0x0274 0x02be 0x032a 0x0299 0x02a8 0x039c 0x05ef 0x0605 0x04fa 0x03d9 0x02b6 0x01d1 0x0113 0x0079 0xffd8 0x001d 0x000d 0xffdf 0xffbb 0xffac 0xffa0 0xff64 0xffc4 0xfff5 0xffee 0xffeb 0x006c 0x005e 0x0044 0x0045 0x02dc 0x0472 0x00db 0xff85 0x0347 0xfdc4 0xf957 0xf7c1 0xf62b 0xf3aa 0xf817 0xfb18 0xfcee 0x01b1 0x079e 0x059d 0x0aa0 0x0608 0x01c6 0xfdaf 0xfdda 0xfa18 0xf6d6 0xf701 0xfc43 0x0186 0x0387 0x0975 0xbb78 0xd771 0xd143 0xbc64 0xbcb9 0xe7f9 0xfd99 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 
Opus demo decode seccussfully.
Opus demo task finished.

Huawei LiteOS #
```