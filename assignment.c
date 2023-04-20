#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>

#define MSG_DELAY_INTERVAL 2

void sigint_handler(int sig);

int main(void){

  signal(SIGINT, sigint_handler);

  int i = 0;
  char msg[100];

  printf("\nEnter message> ");
  fgets(msg, sizeof(msg), stdin);

  while(1){
    
    int pipefds[2];
    pid_t pid;
    char buff[100];
    pipe(pipefds);
    i++;

   if(pipe(pipefds) == -1)
   {
     perror("pipe failed");
     continue;
   }

   pid = fork();

   if(pid<0){
	perror("fork failed\n");
	exit(1);
   }

   else if(pid == 0)
  {
	wait(NULL);
	printf("Child %i (pid= %i)\n", i, getpid());
	close(pipefds[1]); //close write fd

	if(read(pipefds[0], buff, sizeof(buff)) <= 0){
	  perror("read failed");
	  exit(EXIT_FAILURE);
	}

	close(pipefds[0]);
	printf("Child received message '%s' from parent.\n\n", buff); 

	exit(EXIT_SUCCESS);
 }

   else  //parent
  {
	printf("I am parent %d\n", getpid());
	write(pipefds[1], msg, strlen(msg)+1);
	printf("Parent is sending the message... \n\n");

	sleep(MSG_DELAY_INTERVAL); //delay sending message 
     }
  }
  
  return EXIT_SUCCESS;
}

void sigint_handler(int sig){
	printf("\n----Process interrupted, exitting program...----\n");
	exit(1);
}
