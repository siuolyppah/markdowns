![image-20221104200702867](Disk%20Layout.assets/image-20221104200702867.png)



# 柱面扇区寻址（CHS）

需要给出：

1. 柱面号（从0开始索引）
2. 磁头号（从0开始索引）
3. 扇区号**<u>*（从1开始索引）*</u>**









# ==Logical block addressing schema(LBA)==

只需要给出：

1. 一个代表磁盘块的逻辑编号。



# LBA -> CHS

==BIOS只支持CHS寻址，我们需要自行转换==。



两个常数：

- sectors per track/cylinder (on a single side)：每个柱面的扇区数量
- heads per cylinder(or just heads)：每个柱面的磁头个数



sector = (LBA % sectors per track)  + 1 

head = (LAB / sectors per track) % heads

cylinder =(LBA / sectors per track) / heads

![image-20221104202945041](Disk%20Layout.assets/image-20221104202945041.png)







