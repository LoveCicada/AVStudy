### ffmpeg时间戳相关整理（时间基tbr，tbn，tbc）


#### 3: FFmpeg中的时间基与时间戳
##### 3.1 时间基与时间戳
在ffmpeg中，时间基(time_base)是时间戳(timestamp)的单位，时间戳值乘以时间基，可以得到实际的时刻值(以秒等为单位)。

例如，如果一个视频帧的dts是40，pts是160，其time_base是1/1000秒，那么可以计算出此视频帧的解码时刻是40毫秒(40 * 1/1000)，显示时刻是160毫秒(160 * 1/1000)。

FFmpeg中时间戳(pts/dts)的类型是int64_t类型，把一个time_base看作一个时钟脉冲，则可把dts/pts看作时钟脉冲的计数。

##### 3.2 三种时间基tbr、tbn和tbc
不同的封装格式具有不同的时间基。
在FFmpeg处理音视频过程中的不同阶段，也会采用不同的时间基。
FFmpeg中有三种时间基，命令行中tbr、tbn和tbc的打印值是这三种时间基的倒数：
- tbn：对应容器中的时间基。值是AVStream.time_base的倒数
- tbc：对应编解码器中的时间基。值是AVCodecContext.time_base的倒数
- tbr：从视频流中猜算得到，可能是帧率或场率（帧率的2倍）

##### 3.3 内部时间基AV_TIME_BASE
除以上三种时间基外，FFmpeg还有一个内部时间基AV_TIME_BASE(以及分数形式的AV_TIME_BASE_Q)
```C
 // Internal time base represented as integer
 #define AV_TIME_BASE 1000000 //微妙
 // Internal time base represented as fractional value
 #define AV_TIME_BASE_Q (AVRational){1, AV_TIME_BASE}
```
AV_TIME_BASE及AV_TIME_BASE_Q⽤于FFmpeg内部函数处理，使⽤此时间基计算得到时间值表示的 是微秒。

##### 3.4 时间值形式转换
`av_q2d()`将时间从AVRational形式转换为double形式。
AVRational是分数类型，double是双精度浮点数类型，转换的结果单位是秒。
转换前后的值基于同一时间基，仅仅是数值的表现形式不同而已。
- av_q2d()
```C
static inline double av_q2d(AVRational a){
    return a.num / (double)a.den;
}

//av_q2d()使用方法
AVStream stream;
AVPacket packet;
//packet播放时刻值(单位秒)
timestamp = packet.pts * av_q2d(stream.time_base);

//packet播放时长值(单位秒)
duration = packet.duration * av_q2d(stream.time_base);
```

##### 3.5 时间基转换函数
`av_rescale_q()`用于不同时间基的转换，用于将时间值从一种时间基转换为另一种时间基。
将a数值由bq时间基转成cq的时间基
通过返回结果获取以cq时间基表示的新数值。
```C
int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq) av_const;
```

`av_rescale_rnd()`是计算 " a * b /c "的值并分五种方式来取整
```C
int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c, enum AVRounding rnd);
// 它的作⽤是计算 "a * b / c" 的值并分五种⽅式来取整
AV_ROUND_ZERO = 0, 		// Round toward zero. 趋近于0， round(2.5) 为 2, ⽽round(-2.5) 为 -2
AV_ROUND_INF = 1, 		// Round away from zero. 趋远于0 round(3.5)=4, round(-3.5)=-4
AV_ROUND_DOWN = 2, 		// Round toward -infinity.向负⽆穷⼤⽅向 [-2.9, -1.2, 2.4, 5.6,7.0, 2.4] -> [-3, -2, 2, 5, 7, 2]
AV_ROUND_UP = 3, 		// Round toward +infinity. 向正⽆穷⼤⽅向[-2.9, -1.2, 2.4, 5.6,7.0, 2.4] -> [-2, -1, 3, 6, 7, 3]
AV_ROUND_NEAR_INF = 5, 	// Round to nearest and halfway cases away from zero. 
						// 四舍五⼊,⼩于0.5取值趋向0,⼤于0.5取值趋远于0
```

`av_packet_rescale_ts()`用于将AVPacket中各种时间值从一种时间基转换为另一种时间基。
```C
void av_packet_rescale_ts(AVPacket* pkt, AVRational tb_src, AVRational tb_dst);
```

##### 3.6 转封装过程中的时间基转换
容器中的时间基(AVStream.time_base)
```C
typedef struct AVStream{
    //...
    AVRational time_base;
    //...
    // 转封装过程中的时间基转换
};

AVStream.time_base是AVPacket中pts和dts的时间单位，
输入流与输出流中time_base按如下方式确定：
对于输入流：打开输入文件后，调用avformat_find_stream_info()可获取到每个流中time_base
对于输出流：打开输出文件后，调用avformat_write_header()可根据输出文件封装格式确定每个流的time_base并写入输出文件中。
```

不同封装格式具有不同的时间基，在转封装(将一种封装格式转换为另一种封装格式)过程中，时间基转换相关代码如下：
```C
// 在转封装过程中，时间基转换相关代码如下：
av_read_frame(ifmt_ctx, &pkt);
pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INT|AV_ROUND_PASS_MINMAX);
pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);

// 下面的代码具有和上面代码相同的效果：
// 从输入文件中读取packet
av_read_frame(ifmt_ctx, &pkt);
// 将packet中的各时间值从输入流封装格式时间基转换到输出流封装格式时间基
// in_stream->time_base和out_stream->time_base是容器中的时间基
av_packet_rescale_ts(&pkt, in_stream->time_base, out_stream->time_base);
```

##### 3.7 转码过程中的时间基转换
编解码中的时间基(AVCodecContext.time_base)
```C
typedef struct AVCodecContext {
    //...
    AVRational time_base;
    //...
};

/*
AVCodecContext.time_base是帧率(视频帧)的倒数，每帧时间戳递增1，那么tbc等于帧率。
编码过程中，应由用户设置好此参数。
解码过程中，此参数已过时，建议直接使用帧率倒数用作时间基。

问题：按照此处注释说明，帧率为25的视频流，tbc理应为25，但实际值却为50，不知作何解释？是否tbc已经过时，不具参考意义

在实际使用时，在视频解码过程中，我们不使用AVCodecContext.time_base，而用帧率倒数作时间基在视频编码过程中，我们将AVCodecContext.time_base设置为帧率的倒数。
*/
```

###### 3.7.1 视频流
视频按帧播放，所以解码后的原始视频帧时间基为1/framerate
视频解码过程中的时间基转换处理(该段没有参考意义，packet中的pts到底是什么，要看实际的情况，从av_read_frame读取的packet，是以AVStream->time_base，送给解码器之前没有必要转成AVCodecContext->time_base，需要注意的是avcodec_receive_frame后以AVStream->time_base为单位即可。)
```C
// 视频解码时间基转换处理
AVFormatContext *ifmt_ctx;
AVStream *in_stream;
AVCodecContext *dec_ctx;
AVPacket packet;
// 从输入文件中读取编码帧
av_read_frame(ifmt_ctx, &packet);
// 时间基转换
int raw_video_time_base = av_in_q(dec_ctx->framerate);
av_packet_rescale_ts(packet, in_stream->time_base, raw_video->time_base);
// 解码
avcodec_send_packet(dec_ctx, packet);
avcodec_receive_frame(dec_ctx, frame);
```

视频编码过程中的时间基转换处理(编码的时候frame如果以AVStream为time_base送编码器，则avcodec_receive_packet读取的时候也是以转成AVStream->time_base，本质来讲就是具有情况具体分析，不能硬套流程)

```C
// 视频编解码时间基转换处理
AVFormatContext* ofmt_ctx;
AVStream* out_stream;
AVCodecContext* dec_ctx;
AVCodecContext* enc_ctx;
AVPacket packet;
AVFrame *frame;
// 编码
avcodec_send_frame(enc_ctx, frame);
avcodec_receive_packet(enc_ctx, packet);
// 时间基转换
packet.stream_index = out_stream_idx;
enc_ctx->time_base = av_in_q(dec_ctx->framerate);
av_packet_rescale_ts(&packet, enc_ctx->time_base, out_stream->time_base);
// 将编码写入输出媒体文件
av_interleaved_write_frame(ofmt_ctx, &packet);
```

###### 3.7.2 音频流
对于⾳频流也是类似的：

===> ⽐如ffplay 解码播放时就是AVSteam的time_base为基准的packet进⼊到编码器，然后出来的frame再⽤AVSteam的 time_base讲对应的pts转成秒，

===>但是要注意的是ffplay做了⼀个⽐较隐秘的设置：avctx- >pkt_timebase = ic->streams[stream_index]->time_base;

===>即是对应的codeccontext⾃⼰对 pkt_timebase设置和AVStream⼀样的time_base。

​⾳频按采样点播放，所以解码后的原始⾳频帧时间基为 1/sample_rate：

​⾳频解码过程中的时间基转换处理：
```C
//⾳频解码过程中的时间基转换处理：
AVFormatContext *ifmt_ctx;
AVStream *in_stream;
AVCodecContext *dec_ctx;
AVPacket packet;
AVFrame *frame;
// 从输⼊⽂件中读取编码帧
av_read_frame(ifmt_ctx, &packet);
// 时间基转换
int raw_audio_time_base = av_inv_q(dec_ctx->sample_rate);
av_packet_rescale_ts(packet, in_stream->time_base, raw_audio_time_base);
// 解码
avcodec_send_packet(dec_ctx, packet)
avcodec_receive_frame(dec_ctx, frame);  
```

⾳频编码过程中的时间基转换处理：
```C
//⾳频编码过程中的时间基转换处理：
AVFormatContext *ofmt_ctx;
AVStream *out_stream;
AVCodecContext *dec_ctx;
AVCodecContext *enc_ctx;
AVPacket packet;
AVFrame *frame;
// 编码
avcodec_send_frame(enc_ctx, frame);
avcodec_receive_packet(enc_ctx, packet);
// 时间基转换
packet.stream_index = out_stream_idx;
enc_ctx->time_base = av_inv_q(dec_ctx->sample_rate);
av_packet_rescale_ts(&opacket, enc_ctx->time_base, out_stream->time_base);
// 将编码帧写⼊输出媒体⽂件
av_interleaved_write_frame(o_fmt_ctx, &packet);
```