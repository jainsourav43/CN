struct node
{
	int data;
	struct node *next;
} ;

struct lists{
	node* l1;
	node* l2;
};

program LINKEDLIST_PROG{
	version LINKEDLIST_VERS{
		node addlink(lists)=1;
	}=1;
}=0x23451111;
