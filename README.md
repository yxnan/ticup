# ticup
2020年第八届浙江省大学生电子设计竞赛（E题软件部分代码）

基于STM32F103野火开发板

## 工具链

编译：GNU Make + arm-none-eabi-gcc

调试：minicom + pyserial

烧写：[stlink](https://github.com/stlink-org/stlink)

|    目录    |               说明               |
| :--------: | :------------------------------: |
|   debug/   | 串口调试辅助用python、matlab脚本 |
| Libraries/ |   函数库（CMSIS、FWlib、DSP）    |
|   User/    |             用户代码             |

## 编译 & 程序下载

```shell
make && st-flash --format ihex write build/main.hex
```

