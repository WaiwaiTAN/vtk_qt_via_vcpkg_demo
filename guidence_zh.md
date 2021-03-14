# 通过vcpkg安装vtk[qt]教程
## 教程环境
* 操作系统：Microsoft Windows 10 专业版（版本：10.0.19042 N/A Build 19042）
* vcpkg版本：2021-01-13-d67989bce1043b98092ac45996a8230a059a2d7e
* Visual Studio Community 2017版本：15.9.34

## 通过vcpkg安装并集成包vkt[qt]和qt5
使用vcpkg安装包的命令如下：
```powershell
vcpkg install packagename
```
vcpkg执行安装（install）命令会先下载源代码，但网络不顺畅时常常会出现连接错误（SSL connect error）如下：
```powershell
> C:\src\vcpkg\vcpkg.exe install ffmpeg
Computing installation plan...
The following packages will be built and installed:
    ffmpeg[avcodec,avdevice,avfilter,avformat,avresample,core,gpl,postproc,swresample,swscale]:x86-windows -> 4.3.1#11
Detecting compiler hash for triplet x86-windows...
Could not locate cached archive: C:\Users\lenovo\AppData\Local\vcpkg\archives\50\50f9ea1c758cf4148a9bd05fa369bbb5c20728cf.zip
Starting package 1/1: ffmpeg:x86-windows
Building package ffmpeg[avcodec,avdevice,avfilter,avformat,avresample,core,gpl,postproc,swresample,swscale]:x86-windows...
-- Downloading https://github.com/ffmpeg/ffmpeg/archive/n4.3.1.tar.gz -> ffmpeg-ffmpeg-n4.3.1.tar.gz...
-- Downloading https://github.com/ffmpeg/ffmpeg/archive/n4.3.1.tar.gz... Failed. Status: 35;"SSL connect error"
```
这时需要找到出现错误的下载链接（上面的例子中，出现错误的下载链接就是：`"https://github.com/ffmpeg/ffmpeg/archive/n4.3.1.tar.gz"`），将下载链接复制到浏览器或各种下载器中继续下载。
并将下载好的文件更名为vcpkg设定的名字（上面的例子中，vcpkg设定的名字就是`"ffmpeg-ffmpeg-n4.3.1.tar.gz"`），再拷贝到vcpkg的缓存文件目录：`($vcpkgDir)\downloads`下。重新执行安装命令，vcpkg就会从缓存的文件提取源代码进行编译和安装。

如果需要指定所安装库目标平台（如Windows-64bits）的，可在`packagename`后面加上`":x64-windows"`。

在powershell中键入如下命令即可安装vtk[qt]和qt5。
```powershell
vcpkg install vtk[qt] qt5
```

再用集成（integrate）命令将库与Visual Studio集成：
```powershell
vcpkg integrate install
```

## 集成到Visual Studio中并使用qmake
为Visual Studio安装插件[Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123)，并在Qt VS Tools -> Qt Versions中新建Qt version（\<add new Qt version\>），选择路径（Path）为`($vcpkgDir)\installed\($platformTar)\tools\qt5`。这里`$platformTar`指的是所安装库的目标平台。

需要注意的是，编译生成解决方案的时候需要使用VS中的Release配置，因为这里我们没有安装Debug配置所需要的依赖库<sup id="a1">[1](#f1)</sup>。

## Demo运行
VTK官网上给了一些使用VTK和Qt的例子，现来运行例子：[SideBySideRenderWindowsQt](https://vtk.org/Wiki/VTK/Examples/Cxx/Qt/SideBySideRenderWindowsQt)。
新建项目选择Qt -> Qt Widgets Application，按选择qmake创建项目，使用上述例子中的代码，编译生成解决方案即可。

需要注意的是，在一些vtk的头文件中可能会有`#include <vtkCommand.h>`这样的片段，而头文件"vtkCommand.h"并不在集成到VS的时候加入的包含路径`($vcpkgDir)\installed\($platformTar)\include`之下，这样就会导致编译时报找不到头文件的错误。此时如果找不到文件与包含它的文件在同一目录下，可以修改`#include <vtkCommand.h>`为`#include "vtkCommand.h"`却确保头文件能够正确包含。

本Demo已经上传到Github，详请参阅：https://github.com/WaiwaiTAN/vtk_qt_via_vcpkg_demo

## 参考资料
<b id="f1">1</b> [stephane et al.](https://stackoverflow.com/questions/36686316/cannot-find-c-users-qt-work-install-lib-qt5widgetsd-lib-when-compiling-qtwebeng) Cannot find c:\Users\qt\work\install\lib\Qt5Widgetsd.lib when compiling QtWebEngine examples[↩](#a1)