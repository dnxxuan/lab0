#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
void waiting(),stop(),alarming();
int wait_mark;
main()
{
	int p1,p2;
	if(p1=fork())  //创建子进程p1
	{
		if(p2=fork())//p2
		{
			wait_mark=1;
			signal(SIGINT,stop);//接收^C，转stop
			//signal(SIGALRM,alarming);//接收SIGALRM
			waiting();

			kill(p1,16);//向P1发软中断信号16
			kill(p2,17);//向P2发软中断信号17

			wait(0);    //同步
			wait(0);
			printf("parent process is killed\n");
			exit(0);
		}
		else
		{
			wait_mark=1;
			signal(17,stop);
			//signal(SIGINT,SIG_IGN);//忽略^C信号
                              //  signal(SIGQUIT,SIG_IGN);
			while(wait_mark!=0);
			lockf(1,1,0);
			printf("child process2(pid=%d,ppid=%d) is killed by parent!\n",getpid(),getppid());
			lockf(1,0,0);
			exit(0);
		}
	}
	else
	{
		wait_mark=1;
	    signal(16,stop);
		//signal(SIGINT,SIG_IGN);//忽略^C信号
                //signal(SIGQUIT,SIG_IGN);               
		while(wait_mark!=0);
		lockf(1,1,0);
		printf("child process1(pid=%d,ppid=%d) is killed by parent!\n",getpid(),getppid());
		lockf(1,0,0);
		exit(0);
	}
}
void waiting()
{
	sleep(5);
	if(wait_mark!=0)
		kill(getpid(),SIGALRM);
}
void alarming()
{
	wait_mark=0;
}
void stop()
{
	wait_mark=0;
}


