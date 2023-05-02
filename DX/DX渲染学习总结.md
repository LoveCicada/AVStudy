
### 渲染架构
![render_framework](./img/render_framework.jpg)

![DX双显卡渲染](./img/DX-multiGPURender.jpg)

![DX跨进程共享纹理](./img/DX-multiProcessSharedTexture.jpg)

![将共享句柄传递给其他显卡](./img/sharedHanlde-multiGPU.jpg)


### 渲染管线

- 输入装配阶段（IA）
```
input assembler stage
```

- 顶点着色器阶段（VS）
```
vertex shader stage
```

- 外壳着色器阶段（HS）
```
hull shader stage
```

- 曲面细分阶段（TS）
```
tessellator stage
```

- 域着色器阶段（DS）
```
domain shader stage
```

- 几何着色器阶段（GS）
```
geometry shader stage
```

- 流输出阶段（SO）
```
stream output stage
```

- 光栅化阶段（RS）
```
rasterizer stage
```

- 像素着色器阶段（PS）
```
pixel shader stage
```

- 输出合并阶段（OM）
```
output merger stage
```

***

### 常用渲染管线流程
- 输入装配阶段（IA）
```
input assembler stage
```

- 顶点着色器阶段（VS）
```
vertex shader stage
```

- 光栅化阶段（RS）
```
rasterizer stage
```

- 像素着色器阶段（PS）
```
pixel shader stage
```

- 输出合并阶段（OM）
```
output merger stage
```