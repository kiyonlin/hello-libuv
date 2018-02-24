# 设计概述
libuv是最初为NodeJS编写的跨平台支持库。它围绕事件驱动的异步I/O模型进行设计。

该库提供的不仅仅是对不同I/O轮询机制的简单抽象：'handles'和'streams'为套接字和其他实体提供了高级抽象; 还提供了跨平台的文件I/O和线程功能。

下面是一个图表，说明组成libuv的不同部分以及它们与哪些子系统相关：
![libuv组成](architecture.png)

## 句柄和请求

libuv为用户提供2个抽象来处理事件循环：句柄和请求。

句柄代表能够在激活时执行某些操作的长期对象。下面是一些例子：

- 当每次循环迭代处于激活状态时，准备句柄都会调用一次回调函数。
- 每次有新连接时，都会调用一个TCP服务器句柄，以获取其连接回调。

请求代表（通常）短暂的操作。这些操作可以在句柄上执行：写入请求用于在句柄上写入数据; 或独立：getaddrinfo请求不需要直接在循环中运行的句柄。

## I/O循环

The I/O (or event) loop is the central part of libuv. It establishes the content for all I/O operations, and it’s meant to be tied to a single thread. One can run multiple event loops as long as each runs in a different thread. The libuv event loop (or any other API involving the loop or handles, for that matter) is not thread-safe except where stated otherwise.

I/O（或事件）循环是libuv的核心部分。 它为所有I/O操作建立内容，并且它意味着绑定到单个线程。 只要每个循环运行在不同的线程中，就可以运行多个事件循环。 除非另有说明，libuv事件循环（或涉及循环或句柄的任何其他API）不是线程安全的。

The event loop follows the rather usual single threaded asynchronous I/O approach: all (network) I/O is performed on non-blocking sockets which are polled using the best mechanism available on the given platform: epoll on Linux, kqueue on OSX and other BSDs, event ports on SunOS and IOCP on Windows. As part of a loop iteration the loop will block waiting for I/O activity on sockets which have been added to the poller and callbacks will be fired indicating socket conditions (readable, writable hangup) so handles can read, write or perform the desired I/O operation.

事件循环遵循通常的单线程异步I/O方法：所有（网络）I/O在非阻塞套接字上执行，使用给定平台上可用的最佳机制进行轮询：Linux上的epoll，OSX上的kqueue和 其他BSD，SunOS上的事件端口和Windows上的IOCP。 作为循环迭代的一部分，循环将阻止等待已添加到轮询器的套接字上的I/O活动，并且将触发指示套接字条件（可读，可写挂断）的回调，以便句柄可以读取，写入或执行期望的I/O操作。

In order to better understand how the event loop operates, the following diagram illustrates all stages of a loop iteration:
为了更好地理解事件循环如何操作，下图说明了循环迭代的所有阶段：

![I/O循环](loop_iteration.png)

1. 更新循环的'当前时间'。 事件循环会在事件循环计时开始时缓存当前时间，以减少与时间相关的系统调用次数。
- 如果循环处于*活动状态*，则开始迭代，否则将立即退出循环。 如果一个循环具有活动和被引用句柄，活动请求或关闭句柄，它才被认为是*活着*的。
- 运行适时定时器。 所有活动计时器都是在循环的'当前时间'被调用之前计划好的。
- 调用挂起回调。 所有I/O回调函数都是在轮询I/O之后调用的。 然而，有些情况下，在下一次循环迭代中会调用这样的回调。 如果以前的迭代推迟了任何I/O回调，它将在此时运行。
- 调用idle句柄回调。 idle句柄运行在每个循环迭代，当且仅当们是活动的。
- 调用prepare句柄回调。 prepare句柄在循环将阻塞I/O之前调用它们的回调函数。
- 轮询超时计算。 在阻塞I/O之前，循环计算应该阻塞多长时间。 这些是计算超时时间的规则：
	* 如果循环使用UV_RUN_NOWAIT标志运行，则超时值为0。
	* 如果循环将被停止（调用`uv_stop()`），则超时为0。
	* 如果没有活动的句柄或请求，则超时值为0。
	* 如果有任何idle句柄处于活动状态，则超时值为0。
	* 如果有任何待处理的句柄被关闭，则超时值为0。
	* 如果以上任何一种情况都不匹配，则取最近的定时器超时，或者如果没有活动定时器，则为无穷大。
- I/O的循环块。 此时，循环将在上一步计算的持续时间内阻塞I/O。 所有监视给定文件描述符以进行读取或写入操作的I/O相关句柄都会在此时调用它们的回调函数。
- 调用check句柄回调。 check句柄在循环被I/O阻塞之后调用正确的回调函数。 check句柄基本上是prepare句柄的对应物。
- 调用关闭回调。 如果一个句柄通过调用`uv_close()`来关闭，它将得到关闭的回调函数。
- 特殊情况下，循环运行`UV_RUN_ONCE`，因为它意味着向前处理。 在阻塞I/O之后可能没有I/O回调被触发，但是有一段时间已经过去了，所以可能会有定时器到期，这些定时器会调用它们的回调。
- 迭代结束。 如果循环以`UV_RUN_NOWAIT`或`UV_RUN_ONCE`模式运行，则迭代结束并返回`uv_run()`。 如果循环使用`UV_RUN_DEFAULT`运行，它将从循环起始继续运行。

`uv_run()`的源代码：

```C
int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  int timeout;
  int r;
  int ran_pending;

  r = uv__loop_alive(loop);
  if (!r)
    uv__update_time(loop);

  while (r != 0 && loop->stop_flag == 0) {
    uv__update_time(loop);
    uv__run_timers(loop);
    ran_pending = uv__run_pending(loop);
    uv__run_idle(loop);
    uv__run_prepare(loop);

    timeout = 0;
    if ((mode == UV_RUN_ONCE && !ran_pending) || mode == UV_RUN_DEFAULT)
      timeout = uv_backend_timeout(loop);

    uv__io_poll(loop, timeout);
```



`重要提示`：libuv使用线程池来实现异步文件I/O操作，但网络I/O**总是**在单个线程中执行，即每个循环的线程。

> 注意：虽然轮询机制不同，但libuv使得执行模型在Unix系统和Windows中保持一致。

## File I/O

与网络I/O不同，libuv不能依赖特定于平台的文件I/O原语，因此当前的方法是在线程池中运行阻塞文件I/O操作。

libuv当前使用全局线程池，所有循环都可以在其上进行排队工作。 目前在此池上运行3种类型的操作：

- 文件系统操作
- DNS功能（getaddrinfo和getnameinfo）
- 用户通过`uv_queue_work()`指定的代码
