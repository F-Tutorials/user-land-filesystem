# user-land-filesystem
The repository is mainly for course project, aiming at file system teaching process.

注意：**为了能在GitHub Classroom上运行，统一在`./fs/myfs`目录下实现同学自己的文件系统**

来自：哈尔滨工业大学（深圳）

1. 实验代码模板仓库（lab）：[F-Tutorials/user-land-filesystem (github.com)](https://github.com/F-Tutorials/user-land-filesystem)
2. 文档仓库（book）：[ftutorials_book: 用于FUSE文件系统开发的指导书 (gitee.com)](https://gitee.com/ftutorials/ftutorials_book)
   - 在线文档访问入口：[ftutorials_book(gitee.io)](http://ftutorials.gitee.io/ftutorials_book/)
3. 实验参考答案仓库（answer）：[F-Tutorials/user-land-filesystem-answer (github.com)](https://github.com/F-Tutorials/user-land-filesystem-answer)
4. 自动测试脚本仓库（test）：[F-Tutorials/user-land-filesystem-test (github.com)](https://github.com/F-Tutorials/user-land-filesystem-test)

# 一个 GitHub Classroom 的Assignment样例

[样例Assignment邀请链接](https://classroom.github.com/a/2boEflco)

在线排行榜：[Grading (f-tutorials.github.io)](https://f-tutorials.github.io/classroom-grading/)

# Usage of this github classroom template

本节介绍老师应如何基于模板创建自己的 GitHub Classroom 及 Assignment。

1. 在 [github classroom](https://classroom.github.com/classrooms) 创建班级后，选择**New assignment**，输入assignment的基础信息。
2. 选择 **template repository** 为 `F-Tutorials/user-land-filesystem`。
3.  Set up autograding and feedback.
4. 基于[classroom-grading-template](https://github.com/os2edu/classroom-grading-template)创建自己的在线排行榜

```
# Test Case 1
Test type: Input/Output test case
Test name: Normal Test
Run command: bash ./compile_and_run_1.sh
Expected Output: Score: 30/30
	Comparison: included
Timeout: 1
```

```
# Test Case 2
Test type: Input/Output test case
Test name: Enhanced Test
Run command: bash ./compile_and_run_2.sh
Expected Output: Score: 34/34
	Comparison: included
Timeout: 1
```
