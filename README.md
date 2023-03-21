# STM32H750VBT6
STM32H750VBT6-CoreBoard


2022/8/27:
使用cache，spi速度不能太快，屏幕不匹配。目前改用为48M
开cache后在内部flash运行lvgl测试程序有160fps,在外部flash有68fps，还没使用dma2d和优化等价还没调至-o3
