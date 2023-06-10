# SI4469

## 1、介绍

> 说明：SI4460驱动目前计划支持的功能如下

- [ ] 超长包发送
- [ ] 超长包接收
- [ ] 低功耗接收

### 1.1 目录结构

> 说明：参考下面表格，整理出 packages 的目录结构

| 名称 | 说明 |
| ---- | ---- |
| docs  | 文档目录 |
| examples | 例子目录，并有相应的一些说明 |
| inc  | 头文件目录 |
| src  | 源代码目录 |
| port | 移植代码目录。如果没有移植代码，可以不需要 |

### 1.2 许可证

hello package 遵循 MIT 许可，详见 `LICENSE` 文件。

### 1.3 依赖
- RT-Thread 5.0+

## 2、如何打开 hello

使用 si4460 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    miscellaneous packages --->
        [*] A hello package
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 hello

> 说明：在这里介绍 package 的移植步骤、使用方法、初始化流程、准备工作、API 等等，如果移植或 API 文档内容较多，可以将其独立至 `docs` 目录下。

在打开 hello package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

* 完整的 API 手册可以访问这个[链接](docs/api.md)
* 更多文档位于 [`/docs`](/docs) 下，使用前 **务必查看**

## 4、注意事项

> 说明：列出在使用这个 package 过程中需要注意的事项；列出常见的问题，以及解决办法。

## 5、联系方式 & 感谢

* 维护：name
* 主页：