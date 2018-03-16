struct Array
{
	int a[50];
	int size;
};

program ARRAY_PROG{
	version ARRAY_VERS{
		int sum(Array)=1;
	}=1;
}=0x23451111;
