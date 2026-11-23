#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();  // 启动控制台
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging

    // 进程与异常
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector

    // 设备中断
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts

    // 文件系统
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk

    userinit();      // first user process
                      // 只是创建并初始化第一个用户进程
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  // 进入调度器
  scheduler();        // 真正选中并运行这个进程
}
