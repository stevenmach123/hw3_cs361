 

#include <stdio.h>
#include <string.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
void handle1(){
  char *str = "caught sigint";
  write(2,str,strlen(str));
  write(2,"\n",1);

}
void handle2(){
  char *str = "caught sigtstp";
  write(2,str,strlen(str));
  write(2,"\n",1);
}

int main(){

  // signal(SIGINT,handle1);
  // signal(SIGTSTP,handle2);
  while(1){
  char line[100];
  char* argsarray[20];
   int pipefds[2];

    int BUFSIZE =100;
    char buf[BUFSIZE];

        //create pipe
        if(pipe(pipefds) == -1){
                perror("pipe");
                exit(EXIT_FAILURE);
        }
        memset(buf,0,BUFSIZE);


  for (int i=0; i < 20; i++){

     argsarray[i] =  malloc(100 * sizeof(char));

  }
  //printf("me is %d ",getpid());
  //print prompt
  printf("CS361 > ");
//read line from terminal
  fgets(line, 500, stdin);
  //signal(SIGTSTP,handle2);
  //signal(SIGINT,handle1);
  //break the string up into words
  char *word = strtok(line, " ");
  int i =0;

    while (word) {

     printf("word: %s\n", word);
    //copy a word to the arg array
     strcpy(argsarray[i], word);
    //get next wo       rd
     word = strtok(NULL, " ");
      i = i + 1;
    }
   argsarray[i] = (char*)0;
    i+=1;
   if(strcmp(argsarray[0],"exit\n") == 0){
            
            exit(0);
    }

    bool special= false;
    if(i>5){
    for(int j =0; j< i;j++){
        if(strcmp(argsarray[2], ";") == 0 || strcmp(argsarray[2], "|")== 0   ){

                special =true;

        }

    }
    }

    int pid = fork();


     if (pid == 0) {
        if(special ==true && strcmp(argsarray[2], ";") == 0 ){          
           int pid2 = fork();
           int status2;
           waitpid(pid2,&status2,0);
           printf("IN CHILD pid %d,getpid() %d",pid,getpid());
           execv(argsarray[0], argsarray);
           exit(0);

        }
        else if(special ==true && strcmp(argsarray[2], "|") == 0){
          int pid2 = fork();
          if(pid2 ==0){
              printf("pid2= 0\n");
              close(pipefds[1]);
              read(pipefds[0],buf,strlen(buf));
              printf("IN CHILD2 parent getpid() %s",buf);
              printf("IN CHILD2 pid %d getpid() %d",pid2, getpid());
              close(pipefds[0]);
              execv(argsarray[0], argsarray);


          }

          else{
                  int status2;
                  close(pipefds[0]);
                  char mypid = getpid();
                  char *sentence = "CHILD| getpid() ";
                  write(pipefds[1],sentence,strlen(sentence) );
                  write(pipefds[1],&mypid, 1);

                   waitpid(pid2,&status2,0);
                  printf("CHILD| %d getpid() with status %d\n",getpid(),WEXITSTATUS(status2));
                  close(pipefds[1]);
                  exit(0);
          }



        }

    else{
           
         execv(argsarray[0], argsarray);
	 exit(0);
     }
    }
     else{
        
        int status;
        waitpid(pid,&status,0);
        printf("pid: %d status: %d\n",pid,WEXITSTATUS(status));

   
    }


   }


}


