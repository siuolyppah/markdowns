# 场景——提高效率

## 环境搭建

- 基准测试工具：JMH[^1]

- 限制CPU核数，实现方式：
  - 使用虚拟机，为其分配核心数
  - 使用msconfig[^2]，为其分配核心数



[^1]: JMH会执行程序预热，多次执行测试并取平均
[^2]:重启比较麻烦

## 代码实现

- 项目创建：

  ```sh
  mvn archetype:generate -DinteractiveMode=false -DarchetypeGroupId=org.openjdk.jmh -
  DarchetypeArtifactId=jmh-java-benchmark-archetype -DgroupId=org.sample -DartifactId=test -
  Dversion=1.0
  ```

- Java代码：

  ```java
  package org.sample;
  import java.util.Arrays;
  import java.util.concurrent.FutureTask;
  import org.openjdk.jmh.annotations.Benchmark;
  import org.openjdk.jmh.annotations.BenchmarkMode;
  import org.openjdk.jmh.annotations.Fork;
  import org.openjdk.jmh.annotations.Measurement;
  import org.openjdk.jmh.annotations.Mode;
  import org.openjdk.jmh.annotations.Warmup;
  @Fork(1)
  @BenchmarkMode(Mode.AverageTime)
  @Warmup(iterations=3)
  @Measurement(iterations=5)
  public class MyBenchmark {
      static int[] ARRAY = new int[1000_000_00];
      static {
          Arrays.fill(ARRAY, 1);
      }
      @Benchmark
      public int c() throws Exception {
          int[] array = ARRAY;
              FutureTask<Integer> t1 = new FutureTask<>(()->{
                  int sum = 0;
                  for(int i = 0; i < 250_000_00;i++) {
                      sum += array[0+i];
                  }
                  return sum;
              });
          FutureTask<Integer> t2 = new FutureTask<>(()->{
              int sum = 0;
              for(int i = 0; i < 250_000_00;i++) {
                  sum += array[250_000_00+i];
              }
              return sum;
          });
          FutureTask<Integer> t3 = new FutureTask<>(()->{
              int sum = 0;
              for(int i = 0; i < 250_000_00;i++) {
                  sum += array[500_000_00+i];
              }
              return sum;
          });
          FutureTask<Integer> t4 = new FutureTask<>(()->{
              int sum = 0;
              for(int i = 0; i < 250_000_00;i++) {
                  sum += array[750_000_00+i];
              }
              return sum;
          });
          new Thread(t1).start();
          new Thread(t2).start();
          new Thread(t3).start();
          new Thread(t4).start();
          return t1.get() + t2.get() + t3.get()+ t4.get();
      }
      @Benchmark
      public int d() throws Exception {
          int[] array = ARRAY;
          FutureTask<Integer> t1 = new FutureTask<>(()->{
              int sum = 0;
              for(int i = 0; i < 1000_000_00;i++) {
                  sum += array[0+i];
              }
              return sum;
          });
          new Thread(t1).start();
          return t1.get();
      }
  }
  ```







## 效果

```sh
C:\Users\lenovo\eclipse-workspace\test>java -jar target/benchmarks.jar
# VM invoker: C:\Program Files\Java\jdk-11\bin\java.exe
# VM options: <none>
# Warmup: 3 iterations, 1 s each
# Measurement: 5 iterations, 1 s each
# Threads: 1 thread, will synchronize iterations
# Benchmark mode: Average time, time/op
# Benchmark: org.sample.MyBenchmark.c
# Run progress: 0.00% complete, ETA 00:00:16
# Fork: 1 of 1
# Warmup Iteration 1: 0.022 s/op
# Warmup Iteration 2: 0.019 s/op
# Warmup Iteration 3: 0.020 s/op
Iteration 1: 0.020 s/op
Iteration 2: 0.020 s/op
Iteration 3: 0.020 s/op
Iteration 4: 0.020 s/op
Iteration 5: 0.020 s/op
Result: 0.020 ±(99.9%) 0.001 s/op [Average]
 Statistics: (min, avg, max) = (0.020, 0.020, 0.020), stdev = 0.000
 Confidence interval (99.9%): [0.019, 0.021]
# VM invoker: C:\Program Files\Java\jdk-11\bin\java.exe
# VM options: <none>
# Warmup: 3 iterations, 1 s each
# Measurement: 5 iterations, 1 s each
# Threads: 1 thread, will synchronize iterations
# Benchmark mode: Average time, time/op
# Benchmark: org.sample.MyBenchmark.d
# Run progress: 50.00% complete, ETA 00:00:10
# Fork: 1 of 1
# Warmup Iteration 1: 0.042 s/op
# Warmup Iteration 2: 0.042 s/op
# Warmup Iteration 3: 0.041 s/op
Iteration 1: 0.043 s/op
Iteration 2: 0.042 s/op
Iteration 3: 0.042 s/op
Iteration 4: 0.044 s/op
Iteration 5: 0.042 s/op
Result: 0.043 ±(99.9%) 0.003 s/op [Average]
 Statistics: (min, avg, max) = (0.042, 0.043, 0.044), stdev = 0.001
 Confidence interval (99.9%): [0.040, 0.045]
# Run complete. Total time: 00:00:20
```





# 场景——限流

## sleep实现

在没有利用 cpu 来计算时，不要让 while(true) 空转浪费 cpu，这时可以使用 yield 或 sleep 来让出 cpu 的使用权

```java
while(true) {
    try {
        Thread.sleep(50);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}
```

> 这样的代码，常见于服务端循环等待请求，并作出响应



- 可以用 wait 或条件变量达到类似的效果
- 不同的是，后两种都需要加锁，并且需要相应的唤醒操作，一般适用于要进行同步的场景
- sleep 适用于无需锁同步的场景





# 场景——同步与异步

## 需要等待结果

### join方法实现同步

```java
static int result = 0;
private static void test1() throws InterruptedException {
    log.debug("开始");
    Thread t1 = new Thread(() -> {
        log.debug("开始");
        sleep(1);
        log.debug("结束");
        result = 10;
    }, "t1");
    t1.start();
    t1.join();
    log.debug("结果为:{}", result);
}
```



输出：

```
20:30:40.453 [main] c.TestJoin - 开始
20:30:40.541 [Thread-0] c.TestJoin - 开始
20:30:41.543 [Thread-0] c.TestJoin - 结束
20:30:41.551 [main] c.TestJoin - 结果为:10
```



评价：

- 需要外部共享变量，不符合面向对象封装的思想

- 必须等待线程结束，`不能配合线程池使用`
