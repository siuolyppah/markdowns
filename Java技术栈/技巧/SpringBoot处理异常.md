[使用枚举简单封装一个优雅的 Spring Boot 全局异常处理！ (qq.com)](https://mp.weixin.qq.com/s/Y4Q4yWRqKG_lw0GLUsY2qw)

# `@ControllerAdvice`和 `@ExceptionHandler`

> 由切面类(注有`@ControllerAdvice`)处理各种异常。



- 异常信息实体类

  用于包装异常信息：

  ```java
  @Getter
  @Setter
  public class ErrorResponse {
      private String message;
      private String errorTypeName;
  
      public ErrorResponse(Exception e) {
          this(e.getClass().getName(), e.getMessage());
      }
  
      public ErrorResponse(String errorTypeName, String message) {
          this.errorTypeName = errorTypeName;
          this.message = message;
      }
  }
  ```

- 自定义异常类型：

  ```java
  public class ResourceNotFoundException extends RuntimeException {
      private String message;
  
      public ResourceNotFoundException() {
          super();
      }
  
      public ResourceNotFoundException(String message) {
          super(message);
          this.message = message;
      }
  
      @Override
      public String getMessage() {
          return message;
      }
  
      public void setMessage(String message) {
          this.message = message;
      }
  }
  ```

- 全局异常处理器：

  ```java
  @ControllerAdvice(assignableTypes = {ExceptionController.class})
  @ResponseBody
  public class GlobalExceptionHandler {
  
      ErrorResponse illegalArgumentResponse = new ErrorResponse(new IllegalArgumentException("参数错误!"));
      ErrorResponse resourseNotFoundResponse = new ErrorResponse(new ResourceNotFoundException("Sorry, the resourse not found!"));
  
      @ExceptionHandler(value = Exception.class)// 拦截所有异常, 这里只是为了演示，一般情况下一个方法特定处理一种异常
      public ResponseEntity<ErrorResponse> exceptionHandler(Exception e) {
  
          if (e instanceof IllegalArgumentException) {
              return ResponseEntity.status(400).body(illegalArgumentResponse);
          } else if (e instanceof ResourceNotFoundException) {
              return ResponseEntity.status(404).body(resourseNotFoundResponse);
          }
          return null;
      }
  }
  ```



- Controller模拟抛出异常

  ```java
  @RestController
  @RequestMapping("/api")
  public class ExceptionController {
  
      @GetMapping("/illegalArgumentException")
      public void throwException() {
          throw new IllegalArgumentException();
      }
  
      @GetMapping("/resourceNotFoundException")
      public void throwException2() {
          throw new ResourceNotFoundException();
      }
  }
  ```

- 单元测试：

  ```java
  @AutoConfigureMockMvc
  @SpringBootTest
  public class ExceptionTest {
      @Autowired
      MockMvc mockMvc;
  
      @Test
      void should_return_400_if_param_not_valid() throws Exception {
          mockMvc.perform(MockMvcRequestBuilders.get("/api/illegalArgumentException"))
                  .andExpect(status().is(400))
                  .andExpect(jsonPath("$.message").value("参数错误!"));
      }
  
      @Test
      void should_return_404_if_resourse_not_found() throws Exception {
          mockMvc.perform(MockMvcRequestBuilders.get("/api/resourceNotFoundException"))
                  .andExpect(status().is(404))
                  .andExpect(jsonPath("$.message").value("Sorry, the resourse not found!"));
      }
  }
  ```

  

# `@ResponseStatus` 与ResponseStatusException

> 抛出注有`@ResponseStatus`注解的异常



- 异常类：将异常映射为状态码

  ```java
  @ResponseStatus(code = HttpStatus.NOT_FOUND)
  public class ResourseNotFoundException2 extends RuntimeException {
  
      public ResourseNotFoundException2() {
      }
  
      public ResourseNotFoundException2(String message) {
          super(message);
      }
  }
  ```



- 测试用Controller：

  ```java
  
  @RestController
  @RequestMapping("/api")
  public class ResponseStatusExceptionController {
      @GetMapping("/resourceNotFoundException2")
      public void throwException3() {
          throw new ResourseNotFoundException2("Sorry, the resourse not found!");
      }
  }
  ```

  

> `ResponseStatusException`：和使用 `@ResponseStatus` 实现的效果一样，但不必再定义异常类。



`ResponseStatusException` 提供了三个构造方法：

 ```java
 public ResponseStatusException(HttpStatus status) {
     this(status, null, null);
 }
 
 public ResponseStatusException(HttpStatus status, @Nullable String reason) {
     this(status, reason, null);
 }
 
 public ResponseStatusException(HttpStatus status, @Nullable String reason, @Nullable Throwable cause) {
     super(null, cause);
     Assert.notNull(status, "HttpStatus is required");
     this.status = status;
     this.reason = reason;
 }
 ```

- status ：http status
- reason ：response 的消息内容
- cause ：抛出的异常



# 用枚举进行封装

[使用枚举简单封装一个优雅的 Spring Boot 全局异常处理！ (qq.com)](https://mp.weixin.qq.com/s/Y4Q4yWRqKG_lw0GLUsY2qw)



## 返回的异常信息内容

1. 唯一标示异常的 code
2. HTTP 状态码
3. 错误路径
4. 发生错误的时间戳
5. 错误的具体信息



## 异常处理核心代码

### ErrorCode.java

> 此枚举类中包含了异常的唯一标识、HTTP 状态码以及错误信息



```java
import org.springframework.http.HttpStatus;


publicenum ErrorCode {

    RESOURCE_NOT_FOUND(1001, HttpStatus.NOT_FOUND, "未找到该资源"),
    REQUEST_VALIDATION_FAILED(1002, HttpStatus.BAD_REQUEST, "请求数据格式验证失败");

    privatefinal int code;
	privatefinal HttpStatus status;
	privatefinal String message;

    ErrorCode(int code, HttpStatus status, String message) {
        this.code = code;
        this.status = status;
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public HttpStatus getStatus() {
        return status;
    }

    public String getMessage() {
        return message;
    }

    @Override
    public String toString() {
        return"ErrorCode{" +
                "code=" + code +
                ", status=" + status +
                ", message='" + message + '\'' +
                '}';
    }
}
```



### ErrorReponse.java

>返回给客户端具体的异常对象



```java
import org.springframework.util.ObjectUtils;

import java.time.Instant;
import java.util.HashMap;
import java.util.Map;

publicclass ErrorReponse {
    private int code;
    private int status;
    private String message;
    private String path;
    private Instant timestamp;
    private HashMap<String, Object> data = new HashMap<String, Object>();

    public ErrorReponse() {
    }

    public ErrorReponse(BaseException ex, String path) {
        this(ex.getError().getCode(), ex.getError().getStatus().value(), ex.getError().getMessage(), path, ex.getData());
    }

    public ErrorReponse(int code, int status, String message, String path, Map<String, Object> data) {
        this.code = code;
        this.status = status;
        this.message = message;
        this.path = path;
        this.timestamp = Instant.now();
        if (!ObjectUtils.isEmpty(data)) {
            this.data.putAll(data);
        }
    }

// 省略 getter/setter 方法

    @Override
    public String toString() {
        return"ErrorReponse{" +
                "code=" + code +
                ", status=" + status +
                ", message='" + message + '\'' +
                ", path='" + path + '\'' +
                ", timestamp=" + timestamp +
                ", data=" + data +
                '}';
    }
}
```



### BaseException.java

> 系统中其他异常类的父类



```java
publicabstractclass BaseException extends RuntimeException {
    private final ErrorCode error;
    private final HashMap<String, Object> data = new HashMap<>();

    public BaseException(ErrorCode error, Map<String, Object> data) {
        super(error.getMessage());
        this.error = error;
        if (!ObjectUtils.isEmpty(data)) {
            this.data.putAll(data);
        }
    }

    protected BaseException(ErrorCode error, Map<String, Object> data, Throwable cause) {
        super(error.getMessage(), cause);
        this.error = error;
        if (!ObjectUtils.isEmpty(data)) {
            this.data.putAll(data);
        }
    }

    public ErrorCode getError() {
        return error;
    }

    public Map<String, Object> getData() {
        return data;
    }

}
```



### ResourceNotFoundException.java

> 自定义异常



```java
import java.util.Map;

publicclass ResourceNotFoundException extends BaseException {

    public ResourceNotFoundException(Map<String, Object> data) {
        super(ErrorCode.RESOURCE_NOT_FOUND, data);
    }
}
```



### GlobalExceptionHandler.java

> 全局异常捕获



```java
import com.twuc.webApp.web.ExceptionController;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;
import javax.servlet.http.HttpServletRequest;

@ControllerAdvice(assignableTypes = {ExceptionController.class})
@ResponseBody
publicclass GlobalExceptionHandler {

    // 也可以将 BaseException 换为 RuntimeException
    // 因为 RuntimeException 是 BaseException 的父类
    @ExceptionHandler(BaseException.class)
    public ResponseEntity<?> handleAppException(BaseException ex, HttpServletRequest request) {
        ErrorReponse representation = new ErrorReponse(ex, request.getRequestURI());
        returnnew ResponseEntity<>(representation, new HttpHeaders(), ex.getError().getStatus());
    }
}
```





