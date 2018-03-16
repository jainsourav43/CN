#include<string.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<sys/errno.h>
     
extern int errno;       // error NO.
#define MSGPERM 0600    // msg queue permission
#define MSGTXTLEN 128   // msg text length

int msgqid, rc;
int done;

struct msg_buf {
  long mtype;
  char mtext[MSGTXTLEN];
} msg;


int main(int argc,char **argv)
{
	key_t msgkey =1235;
 	int msgqid =msgget(msgkey, IPC_CREAT);
 	//cout<<msgqid<<endl;
	if (msgqid < 0) 
	{
	perror(strerror(errno));
	printf("failed to create message queue with msgqid = %d\n", msgqid);
	return 1;
 	}
	rc = msgrcv(32769, &msg, sizeof(msg.mtext), 0, 0); 
	if (rc < 0) 
	{
    perror( strerror(errno) );
    printf("msgrcv failed, rc=%d\n", rc);
    return 1;
 	} 
    printf("received msg: %s\n", msg.mtext);
 	
;}
