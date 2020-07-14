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

signal(SIGINT,handle1);
   signal(SIGTSTP,handle2);
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
  //print prompti
  printf("CS361 > ");
//read line from terminal
  fgets(line, 500, stdin);
 // signal(SIGTSTP,handle2);
  //signal(SIGINT,handle1);
  //break the string up into words
  char *word = strtok(line, " ");
  int i =0;

    while (word) {
 //printf("word: %s\n", word);
    //copy a word to the arg array
     strcpy(argsarray[i], word);
    //get next wo       rd
     word = strtok(NULL, " ");
      i = i + 1;
    }
   argsarray[i] = NULL;
    i+=1;
   if(strcmp(argsarray[0],"exit\n") == 0){

            exit(0);
    }

   if(  strchr(argsarray[i-2],'\n') !=NULL ){
         argsarray[i-2][strlen(argsarray[i-2])-1]  ='\0';
   }

    bool special= false;
    bool speciall =false;
    char * arg1[20];
    char * arg2[20];
for (int i=0; i <3; i++){

    arg1[i] =  malloc(100 * sizeof(char));

  }
  for (int i=0; i < 3; i++){

     arg2[i] =  malloc(100 * sizeof(char));

  }

    for(int j=0;j<i-1;j++){
        if(strcmp(argsarray[j], ";") == 0 || strcmp(argsarray[j], "|")== 0   ){
                if(strcmp(argsarray[j], ";") == 0){
                  strcpy(arg1[0],argsarray[0]);

                  strcpy(arg1[1],argsarray[1]);
                   arg1[2]=NULL;
                //printf("step2");
                strcpy(arg2[0],argsarray[3]);
                 strcpy(arg2[1],argsarray[4]);
                 arg2[2] =NULL;
                 // printf("step3");	
 		 }
                if(strcmp(argsarray[j], "|") == 0){
                        speciall =true;
                }

                special =true;

        }

    }
 int sw =0;
    int c=0;
    if(special ==true && speciall==true){
    for(int j=0;j<i-1;j++){
        if(sw ==1){
          strcpy(arg2[c],argsarray[j]);
          c++;
        }
        if(strcmp(argsarray[j], "|")==0){
          arg1[c]= NULL;
          c=0;
          sw =1;
        }
       if(sw ==0){
        strcpy(arg1[c],argsarray[j]);
        c++;
       }
 }
    arg2[c]=NULL;
   }
//printf("%ld",strlen(arg1[1]));
//printf("%ld",strlen(arg1[0]));
//printf("%ld",strlen(arg2[1]));


int pid = fork();
    int mystatus2;
    int mypid2;

     if (pid == 0) {
        if(special ==true && strcmp(argsarray[2], ";") == 0 ){
           int pid2 = fork();

           if(pid2 ==0){
                 printf("IN CHILD2; pid %d,getpid() %d",pid2,getpid());
                 execv(arg2[0], arg2);
                 //execv(argsarray[0], argsarray);
           }
           else{
           int status2;
           waitpid(pid2,&status2,0);
           printf("IN CHILD; pid %d,getpid() %d",pid,getpid());
           execv(arg1[0], arg1);
           //execv(argsarray[0], argsarray);
           }
        }
        else if(special ==true && strcmp(argsarray[2], "|") == 0){
          
         
         
          if(pid ==0){
       //     int status;

             close(pipefds[0]);
             
                  
             
                


               dup2(pipefds[1],STDOUT_FILENO);

            
                 // printf("IN CHILD: c");
                 // write(pipefds[1],"hello",5);  
		
                   //dup2(2,1);
  		//write(pipefds[1],&status,sizeof(int));
                  //write(pipefds[1],&pid,sizeof(int));

                //  printf("CHILD| %d getpid() with status %d\n",getpid(),WEXITSTATUS(status2));
                  //printf("childpid:%d status:%d\n",pid2,WEXITSTATUS(status2));

                  close(pipefds[1]);
                  //close(pipefds[0]);

                  execv(arg1[0], arg1);

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
        if(special ==true && strcmp(argsarray[2], "|") == 0){
         
	  
          mystatus2= status; 
	  mypid2 = pid;
	  int pid2= fork();
	  if(pid2==0){
		   
		  dup2(pipefds[0],STDIN_FILENO);
		  close(pipefds[1]);
	//fdf	 int  n_size = read(pipefds[0],buf,sizeof(buf));
		  //write(1,buf,sizeof(buf));			  
		 // printf("IN CHILD2(%d): %s\n",n_size,buf);
		  execv(arg2[0], arg2);
	  }
	  else{
	  close(pipefds[0]);
	  close(pipefds[1]);
	  waitpid(pid2,&status,0);
	  printf("child2pid:%d status:%d\n",pid2,WEXITSTATUS(status));
          printf("child1pid:%d status:%d\n",mypid2,WEXITSTATUS(mystatus2));
	  
          
	  }

	}
	else{
        printf("pid:%d status:%d\n",pid,WEXITSTATUS(status));
	}

    }


   }


}



