#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void){
    /*Constructor*/
    pr_info("Hello World! \n");
    /*A non 0 return means init_module failed; module can't be loaded*/
    return 0;
}

void cleanup_module(void){
    /*Destructor*/
    pr_info("Goodbye World! \n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Camila Sanchez");
MODULE_DESCRIPTION("Modulo Hola Mundo");