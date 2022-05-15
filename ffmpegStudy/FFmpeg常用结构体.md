### FFmpeg常用结构体

1. 解协议(http, rtsp, rtmp, mms)
```
AVIOContext, URLProtocol, URLContext主要存储视音频使用的协议的类型以及状态。
URLProtocol存储输入视音频使用的封装格式。
每种协议都对应一个URLProtocol结构。
```
2. 解封装(flv, avi, rmvb, mp4)
```
AVFormatContext主要存储视音频封装格式中包含的信息。
AVInputFormat存储输入音视频使用的封装格式。
每种视音频格式都对应一个AVInputFormat结构。
```
3. 解码(h264, mpeg2, aac, mp3)
```
每个AVStream存储一个视频/音频流的相关数据。
每个AVStream对应一个AVCodecContext, 存储该视频/音频流使用解码方式的相关数据。
每个AVCodecContext中对应一个AVCodec, 包含该视频/音频对应的解码器。
每种解码器都对应一个AVCodec结构。
```
4. 存数据
```
视频每个结构一般存储一帧, 音频可能存储好几帧
解码前数据: AVPacket
解码后数据: AVFrame
```
![AVPacket-AVFrame](./img/AVPacket-AVFrame.jpg)


***
1. AVCodec 结构体
```C++
typedef struct AVCodec
{
    // 标示 Codec 的名字, 比如，"h264" "h263" 等。
    const char *name;
    // 标示 Codec 的类型，有 video ，audio 等类型。
    enum CodecType type;
    // 标示 Codec 的 ID，有 CODEC_ID_H264 等。
    enum CodecID id;
    // 标示具体的 Codec 对应的 Context 的 size,如：H264Context。
    int priv_data_size;
    // 以下标示 Codec 对外提供的操作,每一种解码器都会实现这些操作。
    int(*init)(AVCodecContext*);
    int(*encode)(AVCodecContext *, uint8_t *buf, int buf_size, void *data);
    int(*close)(AVCodecContext*);
    int(*decode)(AVCodecContext *, void *outdata, int *outdata_size, uint8_t *buf, int buf_size);
    struct AVCodec *next;
}AVCodec;
 
// H264 的主要结构的初始化如下：
AVCodec ff_h264_decoder = {
    "h264",
    AVMEDIA_TYPE_VIDEO,
    CODEC_ID_H264,
    sizeof(H264Context),
    ff_h264_decode_init,
    NULL,
    ff_h264_decode_end,
    decode_frame
}
```
> 存储解码器信息的结构体
AVCodec是类似COM接口的数据结构，表示视音频编解码器，着重于功能函数，一种媒体类型对应一个AVCodec结构，在程序运行时有多个实例。next变量用于把所有支持的编解码器连接成链表，便于遍历查找；id确定了唯一编解码器；priv_data_size表示具体的Codec对应的Conetext结构大小，比如MsrleContext或TSContext，这些具体的结构定义散落于各个.c文件中，为避免太多的if else类语句判断类型再计算大小，这里就直接指明大小，因为这是一个编译时静态确定的字段，所以放在AVCodec而不是AVCodecContext中。
2. AVCodecContext结构体
```C++
typedef struct AVCodecContext
{
    int bit_rate;
    int frame_number;
    //扩展数据，如 mov 格式中 audio trak 中 aac 格式中 esds 的附加解码信    息。
    unsigned char *extradata;
    //扩展数据的 size
    int extradata_size;
    //视频的原始的宽度与高度
    int width, height; // 此逻辑段仅针对视频
    //视频一帧图像的格式，如 YUV420
    enum PixelFormat pix_fmt;
    //音频的采样率
    int sample_rate;
    //音频的声道的数目
    int channels;
    int bits_per_sample;
    int block_align;
    // 指向相应的解码器，如：ff_h264_decoder
    struct AVCodec *codec;
    //指向具体相应的解码器的 context，如 H264Context
    void *priv_data;
    //公共操作函数
    int(*get_buffer)(struct AVCodecContext *c, AVFrame *pic);
    void(*release_buffer)(struct AVCodecContext *c, AVFrame *pic);
    int(*reget_buffer)(struct AVCodecContext *c, AVFrame *pic);
}AVCodecContext;
```
> 描述解码器上下文的数据结构
AVCodecContext结构表示程序运行的当前Codec使用的上下文，着重于所有Codec共有的属性(并且是在程序运行时才能确定其值)和关联其他结构的字段。extrdata和extrdata_size两个字段表述了相应Codec使用的私有数据；codec字段关联相应的编解码；priv_data字段关联各个具体编解码器独有的属性context，和AVCodec结构中的priv_data_size配对使用。
3. AVInputFormat结构体
```C++
typedef struct AVInputFormat
{
    // 标示 format 的名字, 比如，“mov” “mp4” 等。
    const char *name;
    // 标示具体的 format 对应的 Context 的 size,如：MovContext。
    int priv_data_size;
    //具体的操作函数
    int(*read_probe)(AVProbeData*);
    int(*read_header)(struct AVFormatContext *,AVFormatParameters *ap);
    int(*read_packet)(struct AVFormatContext *, AVPacket *pkt);
    int(*read_close)(struct AVFormatContext*);
    struct AVInputFormat *next;
} AVInputFormat;
// Mov 或 mp4 的主要结构的初始化如下：
AVInputFormat ff_mov_demuxer = {
    "mov,mp4,m4a,3gp,3g2,mj2",
    NULL_IF_CONFIG_SMALL("QuickTime/MPEG-4/Motion JPEG 2000 format"),
    sizeof(MOVContext),
    mov_probe,
    mov_read_header,
    mov_read_packet,
    mov_read_close,
    mov_read_seek,
}
```
> 每种封装格式(例如FLV, MKV, MP4, AVI)对应一个该结构体
AVInputFormat是类似COM接口的数据结构，表示输入文件容器格式，着重于功能函数，一种文件容器格式对应一个AVInputFormat结构，在程序运行时有多个实例。next变量用于把所有支持的输入文件容器格式连接成链表，便于遍历查找。priv_data_size表示具体的文件容器格式对应的Context的大小。本例中是MovContext，这些具体的结构定义散落于各个.c文件中。
4. 


