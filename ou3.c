/* OS-COURCE
/* UMU
/* OU3
/* Tobias Estefors
/* dv13tes@cs.umu.se
/* Benchmark to test i/o schedulers
*/

#include "ou3.h"

#define MAX_THREADS 50

struct thread_time_ops time_thread[MAX_THREADS];
int nrOfTokens;
pthread_mutex_t lock;
char rw;

double getRealTime(){
    struct timeval timecheck;
    gettimeofday(&timecheck,NULL);
    return (double)timecheck.tv_sec *1000 + (double)timecheck.tv_usec/1000;

}
char* getFileName(int id){
	/*creating filenames based on id*/
    char idc[2];
    sprintf(idc,"%d",id);
    char *fileName=malloc(20);
    strcpy(fileName,"myfile");
    strcat(fileName,idc);
    strcat(fileName, ".txt");
    return fileName;
}



void *benchmark(void* temp){
    int id=*((int*)temp);
    printf("%d\n",id );
    pthread_mutex_lock(&lock);
    time_thread[id].start_w_time=getRealTime();
    pthread_mutex_unlock(&lock);
    char collect[nrOfTokens+1];
    FILE *fp=NULL;
    char* fileName=getFileName(id);
    printf("%s\n",fileName);
    fp=fopen(fileName,"w"); /*creating files if they dont exist*/
    if(rw='w'){
        for(int i=0; i < nrOfTokens;i++){
            fputs("Hello this is tokens",fp);
        }
    }
    else{
        fgets(collect,nrOfTokens,fp);
    }
    pthread_mutex_lock(&lock);
    time_thread[id].end_w_time=getRealTime();
    pthread_mutex_unlock(&lock);
    free(fileName);
    free(temp);
    fclose(fp);
    return NULL;

}




int main(int argc, char **argv){
    if (argc<3){
        fprintf(stderr, "%s\n","./ou3 r/w nrthreads charstoread/write" );
        fprintf(stderr, "%s\n", "Example ./ou3 r 10 1000" );
        exit(-1);
    }
    pthread_mutex_init(&lock,NULL);
   
    if(argv[1][0] != 'r' && argv[1][0] !='w' ){
    	fprintf(stderr, "%s\n","first arg should be read/write" );
    	exit(-1);
    }
    if(!isNumber(argv[2]) || !isNumber(argv[3])){
    	fprintf(stderr, "%s\n","nrofthreads and tokens must be numbers" );
    	exit(-1);
    }
    printf("%c\n",argv[1][0] );
    rw=argv[1][0];
    int nrOfThreads=atoi(argv[2]);
    if(nrOfThreads > MAX_THREADS){
        fprintf(stderr, "%s\n","Max 100 threads" );
        exit(-1);
    }
    nrOfTokens=atoi(argv[3]);
    /*setting up threads*/
    pthread_t threads[nrOfThreads];
    for(int i=0; i < nrOfThreads;i++){
        int *id=malloc(sizeof(*id));
        *id=i;
        time_thread[i].id=i;
        time_thread[i].create_time=getRealTime();
        pthread_create(&threads[i],NULL,benchmark,id);
    }
    /*collecting threads*/
    for(int i=0; i < nrOfThreads;i++){
        if(pthread_join(threads[i],NULL)){
            fprintf(stderr, "%s\n","Thread error");
            exit(-1);
        }
        time_thread[i].end_time=getRealTime();
    }
    fprintf(stdout,"%c Test with %d tokens and %d threads, time given in miliseconds\n",rw,nrOfTokens,nrOfThreads );
    fprintf(stdout,"ID-Responsetime-Executiontime----\n");
    double responseAvg=0;
    double executeAvg=0;
    for(int i=0; i < nrOfThreads;i++){
        double response=(time_thread[i].end_time - time_thread[i].create_time);
        double execute=(time_thread[i].end_w_time - time_thread[i].start_w_time);
        responseAvg+=response;
        executeAvg+=execute;
        fprintf(stdout,"%d %.3eMS %.3e \n", time_thread[i].id,response,execute);
    }
    double delStart=getRealTime();
    for(int i=0;i < nrOfThreads;i++){
        char* fileName=getFileName(i);
        remove(fileName);
        free(fileName);
    }
    double delEnd=getRealTime();
    fprintf(stdout,"Average response: %.3e , execute: %.3e, Delete:%e\n", responseAvg/nrOfThreads, executeAvg/nrOfThreads,(delEnd-delStart) );

    

}

bool isNumber(char number[]){
    for (int i = 0;  i < strlen(number); i++){
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}