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

   

## ThreadPoolExecutor

[黑马程序员全面深入学习Java并发编程，JUC并发编程全套教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV16J411h7Rd?p=209)



## Fork/Join



# J.U.C并发工具类

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

