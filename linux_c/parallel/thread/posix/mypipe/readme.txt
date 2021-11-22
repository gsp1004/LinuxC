这些代码实现了一个简单的pipe管道。
可以将这些代码进行二次封装来实现如下函数：
open：封装mypipe_init和mypipe_register。并且创建结构体来保存读写权限
read：封装mypipe_read
write：封装mypipe_write
close：封装mypipe_unregister和mypipe_destroy

These codes implement a simple pipe pipeline.
These codes can be encapsulated twice to implement the following functions:
open: Encapsulate mypipe_init and mypipe_register. And create a structure to save read and write permissions
read: Encapsulate mypipe_read
write: Encapsulate mypipe_write
close: encapsulate mypipe_unregister and mypipe_destroy 
