#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void my_print(int pid, long start, long finish) {
    static const long BASE = 1000000000;
    printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start / BASE, start % BASE, finish / BASE, finish % BASE);
}
