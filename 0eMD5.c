#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define LENGTH 20
#define START_POS 11
#define END_POS 19
#define THREADS 4

int main(){
    unsigned char digest[16];
    char string[21]="TestString_000000000";
    MD5_CTX context;
    int i,p;
    for(p=0;p<THREADS;p++){
        if(fork()==0){
            string[START_POS]='0'+p;
            fprintf(stderr,"Process %d started!\n",p);
            while(1){
                MD5_Init(&context);
                MD5_Update(&context,string,LENGTH);
                MD5_Final(digest,&context);
                if(digest[0]==0x0e){
                    for(i=1;i<16;i++){
                        if((digest[i]&0x0f)>=10||(digest[i]>>4)>=10){
                            break;
                        }
                    }
                    if(i==16){
                        puts("**********");
                        printf("string: %s\n",string);
                        printf("md5: ");
                        for(i=0;i<16;i++){
                            printf("%02x",digest[i]);
                        }
                        printf("\n");
                        puts("**********");
                    }
                }
                i=END_POS;
                while(string[i]=='9'){
                    if(i>START_POS+1){
                        string[i]='0';
                        i--;
                    }else{
                        fprintf(stderr,"Process %d finished!\n",p);
                        exit(0);
                    }
                }
                string[i]++;
                if(i<=END_POS-7){
                    fprintf(stderr,"Process %d: %s\n",p,string);
                }
            }
        }
    }
    while(wait(NULL)>0);
    return 0;
}
