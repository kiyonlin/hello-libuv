# 概述
libuv是一个多平台支持库，专注于异步I/O。它主要是为Node.js使用而开发的，但Luvit，Julia，Pyuv等也使用它。

# 特征
- 全功能事件循环，由epoll，kqueue，IOCP，事件端口支持。
- 异步TCP和UDP套接字
- 异步DNS解析
- 异步文件和文件系统操作
- 文件系统事件
- ANSI转义码控制TTY
- 套接字共享IPC，使用Unix域套接字或命名管道（Windows）
- 子进程
- 线程池
- 信号处理
- 高精度时钟
- 线程和同步原语