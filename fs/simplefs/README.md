# Sample FS (SFS)

## 前言

文件系统VFS涉及内核开发，但是内核调试却是一个不小的工作。为此，FUSE (Filesystem in User SpacE) 应运而生。FUSE支持用户态文件系统开发，给文件系统开发者带来福音。本次实验目标为基于FUSE开发一个青春版EXT2文件系统,此分支为Windows移植版本。

## 参考资料

[fuse-tutorials](https://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/html/index.html)
[ibm-develop-your-fuse-fs](https://developer.ibm.com/articles/l-fuse/?mhsrc=ibmsearch_a&mhq=FUSE)
[myfs](../samples)

## 环境配置

**实验环境**
- Windows 10
- VSCode 1.60.2
- cmake  3.16.3
- gcc    9.3.0
- gdb    9.2

**必要依赖** 
```
cygwin编译器
```

**推荐的相关插件**
- Better Comment
- C/C++ Intellisense
- Doxygen (用于生成注释)
- CMake Tools 

### .vscode配置
> 可参考如下配置
**launch.json**
```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug SFS",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/sfs-cygwin.exe",
            "args": [
                "m",
                "--device=E:/user-land-filesystem/driver/user_ddriver/build/ddriver",//挂载路径
                "-f",
                "-d",
                "-s"
            ],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "编译SFS",
            "miDebuggerPath": "C:\\cygwin64\\bin\\gdb.exe"//调试器路径
        }
    ]
}
```
**tasks.json**
```json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "编译SFS",
            "command": "gcc",
            "args": [
                "-o",
                "sfs-cygwin",
                "-D_FILE_OFFSET_BITS=64",
                "-I./include",
                "./src/sfs.c",
                "./src/sfs_utils.c", 
                "./src/ddrivercyg.c",
                "-g",
                "cygdokanfuse2.dll"
            ],
            "options": {
                "cwd": "${workspaceFolder}/"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "调试器生成的任务。"
        }
    ],
    "version": "2.0.0"
}
```

### CMakeLists.txt配置
```CMake
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(sfs-fuse VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall --pedantic -g")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(FUSE REQUIRED)
include_directories(${FUSE_INCLUDE_DIR} ./include)
aux_source_directory(./src DIR_SRCS)
add_executable(sfs-fuse ${DIR_SRCS})
target_link_libraries(sfs-fuse ${FUSE_LIBRARIES})
```
注意，拷贝CMake文件夹至工作目录

## FUSE操作

### 挂载与卸载
**挂载**
```shell
./sfs-cygwin.exe m --device=E:/user-land-filesystem/driver/user_ddriver/build/ddriver -f -d -s
```

**卸载**
```
在任务管理器终止进程
```

## 附录 （SFS实现的操作）

- [x] mount
- [x] umount
- [x] touch
- [x] mkdir
- [x] cat
- [x] cp
- [x] cd 
- [x] rm
- [x] rm -r
- [x] mv
- [x] vi（使用vim进行文件读写）
- [x] code（使用VSCode进行文件读写）