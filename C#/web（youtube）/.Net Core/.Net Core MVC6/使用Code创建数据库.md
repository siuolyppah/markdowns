- Category.cs:

  ```csharp
  using System.ComponentModel.DataAnnotations;
  
  namespace BulkyBookWeb.Models
  {
      public class Category
      {
          [Key]
          public int Id { get; set; }
          [Required]
          public string Name { get; set; }
          public  int DisplayOrder { get; set; }
          public DateTime CreatedDateTime { get; set; }= DateTime.Now;
  
      }
  }
  ```

- ApplicationDbContext.cs:

  ```cs
  using BulkyBookWeb.Models;
  using Microsoft.EntityFrameworkCore;
  
  namespace BulkyBookWeb.Data
  {
      public class ApplicationDbContext : DbContext 
      {
          public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
              : base(options)
          {      
          }
  
          // Categories: 表名
          public DbSet<Category> Categories { get; set; }
      }
  }
  ```

- appsettings.json:

  ```json
  {
    "Logging": {
      "LogLevel": {
        "Default": "Information",
        "Microsoft.AspNetCore": "Warning"
      }
    },
    "AllowedHosts": "*",
    "ConnectionStrings": {
      "DefaultConnection": "Server=DESKTOP-BDDF8FI;Database=Bulky;Trusted_Connection=True;"
    }
  }
  ```

- Program.cs:

  ```cs
  using BulkyBookWeb.Data;
  using Microsoft.EntityFrameworkCore;
  
  var builder = WebApplication.CreateBuilder(args);
  
  // Add services to the container.
  builder.Services.AddControllersWithViews();
  builder.Services.AddDbContext<ApplicationDbContext>(options => options.UseSqlServer(
      builder.Configuration.GetConnectionString("DefaultConnection")
      )) ;
  
  var app = builder.Build();
  
  // Configure the HTTP request pipeline.
  if (!app.Environment.IsDevelopment())
  {
      app.UseExceptionHandler("/Home/Error");
      // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
      app.UseHsts();
  }
  
  app.UseHttpsRedirection();
  app.UseStaticFiles();
  
  app.UseRouting();
  
  /*app.UseAuthentication();*/
  app.UseAuthorization();
  
  app.MapControllerRoute(
      name: "default",
      pattern: "{controller=Home}/{action=Index}/{id?}");
  
  app.Run();
  ```

  



1. 打开NuGet控制台：

   ![image-20220731145206759](%E4%BD%BF%E7%94%A8Code%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93.assets/image-20220731145206759.png)

2. 输入：

   ```sh
   add-migration AddCategoryToDatabase
   ```

   > 需要安装：
   >
   > ![image-20220731145518014](%E4%BD%BF%E7%94%A8Code%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93.assets/image-20220731145518014.png)

3. 输入：

   ```sh
   update-database
   ```

   



