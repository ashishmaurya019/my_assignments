#include<stdio.h>
int main()
{
	int a,b,t=0,r=0;
	printf("enter the 2 no.s\n");
	scanf("%d%d",&a,&b);
	while(b!=0)
	{
		t=b;
		b=a%b;
		a=t;
	}
	r=a;
	printf("GCD is %d\t\n",r);
	return 0;
}

