
[TOC]

### FFmpeg解封装流程

1. 分配解复用器上下文`avformat_alloc_context()`
2. 根据url打开本地文件或网络流`avformat_open_input()`
3. 读取媒体的部分数据包以获取码流信息`avformat_find_stream_info`
4. 读取码流信息: 循环处理
   4.1 从文件中读取数据包`av_read_frame()`
   4.2 定位文件`avformat_seek_file()`或`av_seek_frame()`
5. 关闭解复用器`avformat_close_input()`





