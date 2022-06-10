# 线程池

## 自定义线程池

![image-20220604203209097](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220604203209097.png)



步骤如下：

1. 自定义拒绝策略接口

   ```java
   @FunctionalInterface // 拒绝策略
   interface RejectPolicy<T> {
       void reject(BlockingQueue<T> queue, T task);
   }
   ```

2. 自定义任务队列

   ```java
   public class BlockingQueue<T> {
   
       // 任务队列
       private Deque<T> deque = new ArrayDeque<>();
   
       private int capacity;
   
       private ReentrantLock lock = new ReentrantLock();
   
       // 生产者条件变量
       private Condition fullWaitSet = lock.newCondition();
       // 消费者条件变量
       private Condition emptyWaitSet = lock.newCondition();
   
       public BlockingQueue(int capacity) {
           this.capacity = capacity;
       }
   
       // 带超时时间的阻塞获取
       public T poll(long timeout, TimeUnit timeUnit) {
           lock.lock();
           try {
               long nanos = timeUnit.toNanos(timeout);
               while (deque.isEmpty()) {
                   try {
                       // 返回值：为剩余应等待时间
                       if (nanos <= 0) {
                           return null;
                       } else {
                           nanos = emptyWaitSet.awaitNanos(nanos);
                       }
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
   
               T t = deque.removeFirst();
               fullWaitSet.signalAll();
               return t;
           } finally {
               lock.unlock();
           }
       }
   
       // 阻塞获取
       public T take() {
           lock.lock();
           try {
               while (deque.isEmpty()) {
                   try {
                       emptyWaitSet.await();
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
   
               T t = deque.removeFirst();
               fullWaitSet.signalAll();
               return t;
           } finally {
               lock.unlock();
           }
       }
   
       // 带超时时间的阻塞添加
       public boolean offer(T element, long timeout, TimeUnit timeUnit) {
           lock.lock();
           try {
               long nanos = timeUnit.toNanos(timeout);
               while (deque.size() == capacity) {
                   try {
                       if (nanos <= 0) {
                           return false;
                       } else {
                           nanos = fullWaitSet.awaitNanos(nanos);
                       }
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
   
               deque.addLast(element);
               emptyWaitSet.signalAll();
               return true;
           } finally {
               lock.unlock();
           }
       }
   
   
       // 阻塞添加
       public void put(T element) {
           lock.lock();
           try {
               while (deque.size() == capacity) {
                   try {
                       fullWaitSet.await();
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
               }
   
               deque.addLast(element);
               emptyWaitSet.signalAll();
           } finally {
               lock.unlock();
           }
       }
   
       public int size() {
           lock.lock();
           try {
               return deque.size();
           } finally {
               lock.unlock();
           }
       }
   
       public void tryPut(RejectPolicy<T> rejectPolicy, T task) {
           lock.lock();
           try {
               if (deque.size() == capacity) {
                   // 当任务已满时，将具体的逻辑下放
                   rejectPolicy.reject(this, task);
               } else {
                   deque.addLast(task);
                   emptyWaitSet.signalAll();
               }
           } finally {
               lock.unlock();
           }
       }
   }
   ```

3. 自定义线程池

   ```java
   @Slf4j(topic = "c.ThreadPool")
   public class ThreadPool {
   
       private BlockingQueue<Runnable> taskQueue;
   
       private final HashSet<Worker> workers = new HashSet();
   
       private int coreCounts;
       private TimeUnit timeUnit;
       private long timeout;
   
       private RejectPolicy<Runnable> rejectPolicy;
   
       public ThreadPool(int coreCounts, long timeout, TimeUnit timeUnit, int queueCapacity, RejectPolicy<Runnable> rejectPolicy) {
           this.coreCounts = coreCounts;
           this.timeUnit = timeUnit;
           this.timeout = timeout;
           this.rejectPolicy = rejectPolicy;
           this.taskQueue = new BlockingQueue<>(queueCapacity);
       }
   
       public void execute(Runnable task) {
           synchronized (workers) {
               if (workers.size() < coreCounts) {
                   // 当任务数未超过核心线程数时， 直接由Worker对象执行
                   Worker worker = new Worker(task);
                   log.debug("新增Work{},其执行的任务为：{}", worker, task);
                   workers.add(worker);
                   worker.start();
   
               } else {
                   // 否则，将其加入任务队列
   //                log.debug("任务{}被放入队列", task);
   //                taskQueue.put(task);
   
                   taskQueue.tryPut(rejectPolicy, task);
               }
           }
       }
   
       class Worker extends Thread {
           private Runnable task;
   
           public Worker(Runnable task) {
               this.task = task;
           }
   
           @Override
           public void run() {
               // 从线程队列中，不断的获取任务并执行
               while (task != null || (task = taskQueue.poll(timeout, timeUnit)) != null) {
                   try {
                       log.debug("正在执行的任务：{}", task);
                       task.run();
                   } catch (Exception e) {
                       e.printStackTrace();
                   } finally {
                       task = null;
                   }
               }
   
               synchronized (workers) {
                   // 线程队列中没有任务了
                   log.debug("Worker {}被移除", this);
                   workers.remove(this);
               }
           }
       }
   
       public static void main(String[] args) {
           ThreadPool threadPool = new ThreadPool(2, 1000, TimeUnit.MILLISECONDS, 10,
                   (queue, task) -> {
                       queue.put(task);
                   });
   
           for (int i = 0; i < 5; i++) {
               int finalI = i;
               threadPool.execute(() -> {
                   log.debug("{}", finalI);
               });
           }
       }
   }
   ```

   

## JDK线程池

### ThreadPoolExecutor

![image-20220605220843079](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220605220843079.png)



#### 线程池状态

ThreadPoolExecutor使用int的**高3位来表示线程池状态**，**低29位表示线程数量**。

| 状态名     | 高3位 | 接收新任务 | 处理阻塞队列任务 | 说明                                      |
| ---------- | ----- | ---------- | ---------------- | ----------------------------------------- |
| RUNNING    | 111   | Y          | Y                |                                           |
| SHUTDOWN   | 000   | N          | Y                | 不会接收新任务，但会处理阻塞队列剩余任务  |
| STOP       | 001   | N          | N                | 会中断正在执行的任务，并抛弃阻塞队列任务  |
| TIDYING    | 010   | -          | -                | 任务全执行完毕，活动线程为0，即将进入终结 |
| TERMINATED | 011   | -          | -                | 终结状态                                  |

这些信息存储在一个原子变量 ctl 中，目的是将线程池状态与线程个数合二为一，这样就可以用一次 cas 原子操作进行赋值。

```java
// c 为旧值， ctlOf 返回结果为新值
ctl.compareAndSet(c, ctlOf(targetState, workerCountOf(c))));

// rs 为高 3 位代表线程池状态， wc 为低 29 位代表线程个数，ctl 是合并它们
private static int ctlOf(int rs, int wc) { return rs | wc; }
```



#### 构造方法

```java
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler)
```

- corePoolSize：核心线程数目 (最多保留的线程数)
- maximumPoolSize：最大线程数目（**核心线程数量 + 救济线程数量**）
- keepAliveTime：生存时间 - 针对救急线程
- unit：时间单位 - 针对救急线程（核心线程不限制生存时间）。
- workQueue：阻塞队列
- threadFactory：线程工厂 - 可以为线程创建时起个好名字
- handler：拒绝策略

![image-20220609220807501](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220609220807501.png)



注意：

- 线程池中刚开始没有线程，当一个任务提交给线程池后，线程池会创建一个新线程来执行任务。（即懒惰创建）

- 当线程数达到 corePoolSize 并没有线程空闲，这时再加入任务，新加的任务会被加入workQueue 队列排 队，直到有空闲的线程。

- 如果队列选择了有界队列，那么任务超过了队列大小时，会创建 maximumPoolSize - corePoolSize 数目的线程来**救急**。

- 如果线程到达 maximumPoolSize 仍然有新任务这时会执行拒绝策略。**拒绝策略** jdk 提供了 4 种实现，其它著名框架也提供了实现：

  - AbortPolicy 让调用者抛出 RejectedExecutionException 异常，这是默认策略

  - CallerRunsPolicy 让调用者运行任务

  - DiscardPolicy 放弃本次任务

  - DiscardOldestPolicy 放弃队列中最早的任务，本任务取而代之

    ![image-20220609221843983](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220609221843983.png)

  - Dubbo 的实现：在抛出 RejectedExecutionException 异常之前会记录日志，并 dump 线程栈信息，方便定位问题

  - Netty 的实现：是创建一个新线程来执行任务

  - ActiveMQ 的实现：带超时等待（60s）尝试放入队列，类似我们之前自定义的拒绝策略

  - PinPoint 的实现：它使用了一个拒绝策略链，会逐一尝试策略链中每种拒绝策略

- 当高峰过去后，超过corePoolSize 的救急线程如果一段时间没有任务做，需要结束节省资源，这个时间由 keepAliveTime 和 unit 来控制。

> 基于此构造方法，JDK的`Executors `类中提供了众多工厂方法来创建各种用途的线程池



#### newFiexedThreadPool

```java
public static ExecutorService newFixedThreadPool(int nThreads) {
    return new ThreadPoolExecutor(nThreads, nThreads,
                                  0L, TimeUnit.MILLISECONDS,
                                  new LinkedBlockingQueue<Runnable>());
}
```

特点：

- 核心线程数与最大线程数相同（即不会创建救急线程）。
- 阻塞队列是无界的，**可以放任意数量的任务**。

> 适合于：任务量已知，相对耗时的任务



 ```java
 @Slf4j(topic = "c.TestThreadPoolExecutors")
 public class TestThreadPoolExecutors {
 
     public static void main(String[] args) {
         ExecutorService pool = Executors.newFixedThreadPool(2, new ThreadFactory() {
             private AtomicInteger t = new AtomicInteger(1);
 
             @Override
             public Thread newThread(Runnable r) {
                 return new Thread(r, "mypool_" + t.getAndIncrement());
             }
         });
 
         pool.execute(()->{
             log.debug("running");;
         });
         pool.execute(()->{
             log.debug("running");;
         });
     }
 }
 
 ```



#### newCachedThreadPool

```java
public static ExecutorService newCachedThreadPool() {
    return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                                  60L, TimeUnit.SECONDS,
                                  new SynchronousQueue<Runnable>());
}
```

特点：

- 创建的全都是救急线程
- 救急线程的数量没有限制（Integer的最大值）

- 采用了`SynchronousQueue`作为任务阻塞队列，其**没有容量**，没有线程来取是放不进去的。

  ```java
  SynchronousQueue<Integer> integers = new SynchronousQueue<>();
  new Thread(() -> {
      try {
          log.debug("putting {} ", 1);
          integers.put(1);
          log.debug("{} putted...", 1);
          
          log.debug("putting...{} ", 2);
          integers.put(2);
          log.debug("{} putted...", 2);
      } catch (InterruptedException e) {
          e.printStackTrace();
      }
  },"t1").start();
  
  sleep(1);
  new Thread(() -> {
      try {
          log.debug("taking {}", 1);
          integers.take();
      } catch (InterruptedException e) {
          e.printStackTrace();
      }
  },"t2").start();
  
  sleep(1);
  new Thread(() -> {
      try {
          log.debug("taking {}", 2);
          integers.take();
      } catch (InterruptedException e) {
          e.printStackTrace();
      }
  },"t3").start();
  
  ```

  

> 适合：任务数比较密集，但每个任务的执行时间较短的情况



#### newSingleThreadExecutor

```java
public static ExecutorService newSingleThreadExecutor() {
    return new FinalizableDelegatedExecutorService
        (new ThreadPoolExecutor(1, 1,
                                0L, TimeUnit.MILLISECONDS,
                                new LinkedBlockingQueue<Runnable>()));
}
```

> 使用常见：希望多个任务**排队执行**。



与自己手动创建一个线程的区别：

- 若自己创建一个线程执行任务，线程将因执行失败（异常）而终止。

  而线程池将新建一个线程，保证池中有一个线程可用。

与`Executors.newFixedThreadPool(1)`的区别：

- fixed线程池，对外暴露的是ThreadPoolExecutor对象，将其强转回其实际类型，即可调用setCorePoolSize等方法修改线程池大小。
- 而single线程池，对外暴露的是FinalizableDelegatedExecutorService对象，无法修改。



#### 提交任务

```java
// 执行任务
void execute(Runnable command);

// 提交任务 task，用返回值 Future 获得任务执行结果
<T> Future<T> submit(Callable<T> task);

// 提交 tasks 中所有任务
<T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks)
    throws InterruptedException;

// 提交 tasks 中所有任务，带超时时间(规定时间没处理的任务将被取消)
<T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks,
                              long timeout, TimeUnit unit)
    throws InterruptedException;

// 提交 tasks 中所有任务，哪个任务先成功执行完毕，返回此任务执行结果，其它任务取消
<T> T invokeAny(Collection<? extends Callable<T>> tasks)
    throws InterruptedException, ExecutionException;

// 提交 tasks 中所有任务，哪个任务先成功执行完毕，返回此任务执行结果，其它任务取消，带超时时间
<T> T invokeAny(Collection<? extends Callable<T>> tasks,
                long timeout, TimeUnit unit)
    throws InterruptedException, ExecutionException, TimeoutException;
```



#### 关闭线程池

- `shutdown`方法：

  ```java
  /*
  线程池状态变为 SHUTDOWN
  - 不会接收新任务
  - 但已提交任务会执行完
  - 此方法不会阻塞调用线程的执行
  */
  void shutdown();
  ```

  ```java
  public void shutdown() {
      final ReentrantLock mainLock = this.mainLock;
      mainLock.lock();
      try {
          checkShutdownAccess();
          // 修改线程池状态
          advanceRunState(SHUTDOWN);
          // 仅会打断空闲线程
          interruptIdleWorkers();
          onShutdown(); // 扩展点 ScheduledThreadPoolExecutor
      } finally {
          mainLock.unlock();
      }
      // 尝试终结(没有运行的线程可以立刻终结，如果还有运行的线程也不会等)
      tryTerminate();
  }
  ```

- `shutdownNow`方法：

  ```java
  /*
  线程池状态变为 STOP
  - 不会接收新任务
  - 会将 队列中 的任务返回
  - 并用 interrupt 的方式中断正在执行的任务
  */
  List<Runnable> shutdownNow();
  ```

  ```java
  public List<Runnable> shutdownNow() {
      List<Runnable> tasks;
      final ReentrantLock mainLock = this.mainLock;
      mainLock.lock();
      try {
          checkShutdownAccess();
          // 修改线程池状态
          advanceRunState(STOP);
          // 打断所有线程
          interruptWorkers();
          // 获取队列中剩余任务
          tasks = drainQueue();
      } finally {
          mainLock.unlock();
      }
      // 尝试终结
      tryTerminate();
      return tasks;
  }
  ```

- 其他方法：

  ```java
  // 不在 RUNNING 状态的线程池，此方法就返回 true
  boolean isShutdown();
  
  // 线程池状态是否是 TERMINATED
  boolean isTerminated();
  
  // 调用 shutdown 后，调用线程并不会等待所有任务运行结束
  // 因此如果想在线程池 TERMINATED 后做些事情，可以利用此方法等待
  boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException;
  ```



#### 设计模式——工作线程

见设计模式（异步模式之工作线程）





#### 任务调度线程池-Timer

在『任务调度线程池』功能加入之前，可以使用 java.util.Timer 来实现定时功能，Timer 的优点在于简单易用，但由于所有任务都是由同一个线程来调度，因此所有任务都是串行执行的，同一时间只能有一个任务在执行，前一个任务的延迟或异常都将会影响到之后的任务。

```java
public static void main(String[] args) {
    Timer timer = new Timer();
    TimerTask task1 = new TimerTask() {
        @Override
        public void run() {
            log.debug("task 1");
            sleep(2); }
    };
    TimerTask task2 = new TimerTask() {
        @Override
        public void run() {
            log.debug("task 2");
        }
    };
    // 使用 timer 添加两个任务，希望它们都在 1s 后执行
    // 但由于 timer 内只有一个线程来顺序执行队列中的任务，因此『任务1』的延时，影响了『任务2』的执行
    timer.schedule(task1, 1000);
    timer.schedule(task2, 1000);
}
```

```
20:46:09.444 c.TestTimer [main] - start... 
20:46:10.447 c.TestTimer [Timer-0] - task 1 
20:46:12.448 c.TestTimer [Timer-0] - task 2 
```



#### 任务调度线程池-ScheduledExecutorService

```java
// 设置的是核心线程数
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) {
    return new ScheduledThreadPoolExecutor(corePoolSize);
}
```



##### 延时执行

例如：
```java
@Slf4j(topic = "c.SchedulePoolTest")
public class SchedulePoolTest {

    public static void main(String[] args) {
        ScheduledExecutorService pool = Executors.newScheduledThreadPool(2);

        pool.schedule(()->{
            log.debug("task1 running");
        },2, TimeUnit.SECONDS);

        pool.schedule(()->{
            log.debug("task2 running");
        },1, TimeUnit.SECONDS);
    }
}
```



##### 定时执行

```java
@Slf4j(topic = "c.SchedulePoolTest")
public class SchedulePoolTest {

    public static void main(String[] args) {
        ScheduledExecutorService pool = Executors.newScheduledThreadPool(2);

        log.debug("begin");
        pool.scheduleAtFixedRate(()->{
            log.debug("running");
        },2,1,TimeUnit.SECONDS);	// 初始延迟2秒，之后每隔1秒执行一次该任务
    }
}
```

```
20:16:17 [main] c.SchedulePoolTest - begin
20:16:19 [pool-1-thread-1] c.SchedulePoolTest - running
20:16:20 [pool-1-thread-1] c.SchedulePoolTest - running
20:16:21 [pool-1-thread-1] c.SchedulePoolTest - running
20:16:22 [pool-1-thread-1] c.SchedulePoolTest - running
```



如何让每周四 18:00:00 定时执行任务？

```java
// 获得当前时间
LocalDateTime now = LocalDateTime.now();
// 获取本周四 18:00:00.000
LocalDateTime thursday = 
    now.with(DayOfWeek.THURSDAY).withHour(18).withMinute(0).withSecond(0).withNano(0);

// 如果当前时间已经超过 本周四 18:00:00.000， 那么找下周四 18:00:00.000
if(now.compareTo(thursday) >= 0) {
    thursday = thursday.plusWeeks(1);
}
// 计算时间差，即延时执行时间
long initialDelay = Duration.between(now, thursday).toMillis();
// 计算间隔时间，即 1 周的毫秒值
long oneWeek = 7 * 24 * 3600 * 1000;
ScheduledExecutorService executor = Executors.newScheduledThreadPool(2);

System.out.println("开始时间：" + new Date());
executor.scheduleAtFixedRate(() -> {
    System.out.println("执行时间：" + new Date());
}, initialDelay, oneWeek, TimeUnit.MILLISECONDS);
```





#### 正确处理任务中的异常

- 方法1：主动捉异常

  ```java
  ExecutorService pool = Executors.newFixedThreadPool(1);
  pool.submit(() -> {
      try {
          log.debug("task1");
          int i = 1 / 0;
      } catch (Exception e) {
          log.error("error:", e);
      }
  });
  ```

- 方法2：使用 Future

  ```java
  ExecutorService pool = Executors.newFixedThreadPool(1);
  Future<Boolean> f = pool.submit(() -> {
      log.debug("task1");
      int i = 1 / 0;
      return true;
  });
  log.debug("result:{}", f.get());
  ```



## Tomcat线程池

![image-20220610204705784](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220610204705784.png)

- LimitLatch 用来限流，可以控制最大连接个数，类似 J.U.C 中的 Semaphore 后面再讲
- Acceptor 只负责【接收新的 socket 连接】
- Poller 只负责监听 socket channel 是否有【可读的 I/O 事件】
- 一旦可读，封装一个任务对象（socketProcessor），提交给 Executor 线程池处理
- Executor 线程池中的工作线程最终负责【处理请求】



> Tomcat 线程池扩展了 ThreadPoolExecutor，行为稍有不同：
>
> 如果总线程数达到 maximumPoolSize：
>
> - 这时不会立刻抛 RejectedExecutionException 异常
> - 而是再次尝试将任务放入队列，如果还失败，才抛出 RejectedExecutionException 异常



## Fork/Join

### 概念

Fork/Join 是 JDK 1.7 加入的新的线程池实现，它体现的是一种分治思想，**适用于能够进行任务拆分的 cpu 密集型运算**。

所谓的任务拆分，是将一个大任务拆分为算法上相同的小任务，直至不能拆分可以直接求解。跟递归相关的一些计 算，如归并排序、斐波那契数列、都可以用分治思想进行求解

Fork/Join 在分治的基础上加入了多线程，可以把每个任务的分解和合并交给不同的线程来完成，进一步提升了运算效率

**Fork/Join 默认会创建与 cpu 核心数大小相同的线程池**



### 使用

提交给 Fork/Join 线程池的任务，需要继承 `RecursiveTask`（有返回值）或 `RecursiveAction`（没有返回值）

例如下面定义了一个对 1~n 之间的整数求和的任务：

```java
@Slf4j(topic = "c.AddTask")
class AddTask1 extends RecursiveTask<Integer> {
    int n;
    
    public AddTask1(int n) {
        this.n = n;
    }
    
    @Override
    public String toString() {
        return "{" + n + '}';
    }
    
    @Override
    protected Integer compute() {
        // 如果 n 已经为 1，可以求得结果了
        if (n == 1) {
            log.debug("join() {}", n);
            return n;
        }

        // 将任务进行拆分(fork)，由一个线程执行
        AddTask1 t1 = new AddTask1(n - 1);
        t1.fork();
        log.debug("fork() {} + {}", n, t1);

        // 合并(join)结果
        int result = n + t1.join();
        log.debug("join() {} + {} = {}", n, t1, result);
        return result;
    }
}
```

然后提交给 ForkJoinPool 来执行：
```java
public static void main(String[] args) {
    ForkJoinPool pool = new ForkJoinPool(4);
    System.out.println(pool.invoke(new AddTask1(5)));
}
```

结果如下：

```
[ForkJoinPool-1-worker-0] - fork() 2 + {1} 
[ForkJoinPool-1-worker-1] - fork() 5 + {4} 
[ForkJoinPool-1-worker-0] - join() 1 
[ForkJoinPool-1-worker-0] - join() 2 + {1} = 3 
[ForkJoinPool-1-worker-2] - fork() 4 + {3} 
[ForkJoinPool-1-worker-3] - fork() 3 + {2} 
[ForkJoinPool-1-worker-3] - join() 3 + {2} = 6 
[ForkJoinPool-1-worker-2] - join() 4 + {3} = 10 
[ForkJoinPool-1-worker-1] - join() 5 + {4} = 15 
15 
```

![image-20220610210804515](JUC%E4%B9%8B%E5%B9%B6%E5%8F%91%E5%B7%A5%E5%85%B7.assets/image-20220610210804515.png)





# J.U.C并发工具类

## AQS 原理

[黑马程序员全面深入学习Java并发编程，JUC并发编程全套教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV16J411h7Rd?p=235&vd_source=be746efb77e979ca275e4f65f2d8cda3)



## Lock

## Semaphore

## CountdownLatch

## CyclicBarrier

## ConcurrentHashMap

## ConcurrentLinkedQueue

## BlockingQueue

## CopyOnWriteArrayList





# 第三方工具类

## disruptor

## guava-RateLimiter

