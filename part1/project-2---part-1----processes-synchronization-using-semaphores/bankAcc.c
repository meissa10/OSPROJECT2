#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>


int  main(){ 
      srand(time(NULL)); 
      int    ShmID, *ShmPTR, i, status;
      pid_t  pid;
      int    Turn = 0;
      int    BankAccount = 0;
      sem_t mutex;
	
		  
  
	   ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0){
          printf("*** shmget error\n");
          exit(1);
     }
  
		 ShmPTR = (int *) shmat(ShmID, NULL, 0);
		 if (*ShmPTR == -1) {
          printf("*** shmat error\n");
          exit(1);
     }
  
  
		 ShmPTR[0] = BankAccount;
     ShmPTR[1] = Turn;
  
  
		 pid = fork();
		 if (pid < 0) { 
        fprintf(stderr, "fork fauilure" );
        return 1; 
    }
  
    // Parent process 
    else if (pid > 0) 
    { 		
      int randSleep = rand() % 5;
			printf("Parent");
		  for( i = 0 ; i < 25 ; i++ ) {
				sem_wait(&mutex);
        
        
				sleep(randSleep);
        
        
				while(ShmPTR[1] != 0) {}
				if (ShmPTR[0] <= 100) {
					
					int localBalance = rand() % 100;
					if ((localBalance % 2) == 0 ) {
						ShmPTR[0] = ShmPTR[0] + localBalance;
						printf("Dear old Dad: Deposits $%d / balance = $%d\n", localBalance, ShmPTR[0]);
					} 
          else {
						printf("Dear old Dad: Doesn't have any money to give\n");
					}
					ShmPTR[1] = 1;
				} 
        else {
						printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", ShmPTR[0]);
						ShmPTR[1] = 1;
				}
				sem_post(&mutex);
     } 
		}
    // child process 
    else{ 
      
			for( i = 0 ; i < 25 ; i++ ) {
				sem_wait(&mutex);
				sleep(rand() % 5);
				while(ShmPTR[1] != 1) {}
				int localBalance = rand() % 50;
				printf("Poor Student needs $%d\n", localBalance);
				if (localBalance <= ShmPTR[0]) {
					
					ShmPTR[0] = ShmPTR[0] - localBalance;
					printf("Poor Student: Withdraws $%d / balance = $%d\n", localBalance, ShmPTR[0]);
				} else {
					printf("Poor Student: Not Enough Cash ($%d)\n", ShmPTR[0] );
				}
				ShmPTR[1] = 0;
				sem_post(&mutex);
			}
		}
}