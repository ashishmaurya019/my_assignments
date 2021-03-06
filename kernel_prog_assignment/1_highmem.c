#include<linux/module.h>
#include<linux/init.h>
#include<linux/gfp.h>
#include<linux/errno.h>
#include<linux/highmem.h>
#include<asm/page.h>

	struct page  *spage;

static int highmem_init(void)
	{
	dump_stack();

	spage=alloc_pages(GFP_KERNEL|__GFP_HIGHMEM,5);

	if(!spage)
	
	return -ENOMEM;

	unsigned  int hadd,i;
	for(i=0;i<32;i++)
	{
	hadd=((unsigned int)kmap(spage+i));
	printk("the logical address of the page is %x\n",hadd);
	}
	return 0;




	}
static void highmem_exit(void)
{
	int i;
	for(i=0;i<32;i++)
	kunmap(spage+i);

	__free_pages(spage,5);
	
	printk("%d Pages freed\n",i);

}

module_init(highmem_init);
module_exit(highmem_exit);

MODULE_LICENSE("GPL");

