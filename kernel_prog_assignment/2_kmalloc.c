#include<linux/module.h>
#include<linux/gfp.h>
#include<linux/init.h>
#include<linux/highmem.h>
#include<linux/slab.h>
#define  MAX 150

static int *slab[MAX];
static unsigned int *ladd;

static int normal_init(void)
{
	int i;
	for(i=0;i<MAX;i++)
	{
	ladd = (unsigned int)kmalloc(256,GFP_KERNEL);
	if(!ladd)
		return -ENOMEM;
	slab[i]=ladd;
	
	printk("Logical address is..%x",slab[i]);
	}
return 0;
}

static void normal_exit(void)
{
	int i;
	for(i=0;i<MAX;i++)
	{
	kfree(slab[i]);
	}
	printk("\n256 slab freed\n");
}

module_init(normal_init);
module_exit(normal_exit);

MODULE_LICENSE("GPL");


