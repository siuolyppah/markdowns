```asm
mov ax, 3
int 0x10    ; 将显示模式设置文本模式

mov ax, 0xb800
mov ds, ax

; 奇数位保存文本样式
mov byte [0], 'H'
mov byte [2], 'E'
mov byte [4], 'L'
mov byte [6], 'L'
mov byte [8], 'O'

halt:
    jmp halt

times 510 - ($ -$$) db 0
db 0x55, 0xaa
```



1. 加电：

   ip和cs，设置为f000

   ![image-20220721005533458](Hello%20World.assets/image-20220721005533458.png)

2. 检测硬件是否正常

3. 寻找可启动的设备

4. 如果可启动设备是硬盘，将硬盘的主引导扇区（编号为0），读取到0x7c00的位置。

5. 调整到0x7c00执行