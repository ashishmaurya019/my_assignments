
#include<linux/module.h>
#include<linux/init.h>
#include<linux/gfp.h>
#include<linux/errno.h>
#include<linux/mm.h>
#include<asm/page.h>
#include<linux/highmem.h>

struct page *spage;

static int lowmem_init(void)
{

	
	dump_stack();

	spage =  alloc_page(GFP_KERNEL|GFP_DMA);
	if(!spage)
		return -ENOMEM;	

//	printk("address of struct page {}...%x\n",spage);
	
	unsigned int ladd;

//	ladd = (unsigned int)page_address(spage);	

	ladd =(unsigned int)kmap(spage);
	 
	printk("logical address of the allocated page=%x\n",ladd);
	ladd++;
	     printk("logical address of the allocated page=%x\n",ladd);

return 0;
}

static void lowmem_exit(void)
{
	
	__free_page(spage);
	
	printk("Pages freed\n");
}

module_init(lowmem_init);
module_exit(lowmem_exit);

MODULE_LICENSE("GPL");

