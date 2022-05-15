### windows下编译ffmpeg

- 通过vcpkg安装
1. 安装vcpkg
- 官方文档
https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md#快速开始-windows

```bash
git clone git@github.com:microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
```

2. 安装ffmpeg
```bash
.\vcpkg.exe install ffmpeg:x64-windows
```

3. ffmpeg动态库生成目录
```
dll
\vcpkg\installed\x64-windows

pdb
vcpkg\buildtrees\ffmpeg\x64-windows-rel
```

4. 安装ffplay
```bash
.\vcpkg.exe install ffmpeg[ffplay]:x64-windows
```