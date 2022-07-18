
- 编译命令
```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A Win32 ..
```

- 不同vs版本
```
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake -G "Visual Studio 15 2017" -A Win32 ..
cmake -G "Visual Studio 14 2015" -A Win32 ..

cmake -G "Visual Studio 17 2022" -A Win64 ..

```

- 注意
```
本案例提供的ffmpeg库为32位
```

- 源码
```
ffmpeg-4.3
```

- ffmpeg dll
```
./bin/
```

- ffmpeg lib
```
./lib/
```

- ffmpeg include
```
./include/
```