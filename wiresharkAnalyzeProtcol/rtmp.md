### 抓包分析srs的rtmp流

- `工具`
```
wireshark
ffmepg
```

- `环境`
```
win11 wsl2
srs流媒体服务器
```

- `推流/拉流命令`
```
ffmpeg -re -i lol.flv -c copy -f flv rtmp://172.23.127.110/live/livestream

ffplay rtmp://172.23.113.130/live/livestream
```

- `学习资料`

[rtmp协议详解](https://www.cnblogs.com/jimodetiantang/p/8974075.html)

[rtmp complex handshake，变更的握手，支持h264/aac](https://winlin.blog.csdn.net/article/details/13006803)

[Rtmp协议复杂握手（handshake）详解](https://blog.csdn.net/qq_28309121/article/details/104647011)

[SRS之RTMP handshake](https://www.cnblogs.com/jimodetiantang/p/9061380.html) 

[SRS4.0 中文文档](https://github.com/ossrs/srs/wiki/v4_CN_Home)