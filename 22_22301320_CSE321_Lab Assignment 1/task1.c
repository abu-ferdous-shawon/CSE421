#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


int n;
int *fibonacci;
int s;

void* fibonacci_sequence(void* arg)
{
    fibonacci = (int*)malloc((n+1)*sizeof(int));
    if (n >= 0){
      fibonacci[0] = 0;
    }
    if (n >= 1){
      fibonacci[1] = 1;
    }
    int current = 0;
    int val1 = fibonacci[1];
    int val2 = fibonacci[0];
    
    for (int i=2; i<=n; ++i){
        current = val1 + val2;
        val2 = val1;
        val1 = current;
        
        fibonacci[i] = current;
      }
    
  pthread_exit(NULL);
}

void* fibonacci_search(void* arg){
    int *search = (int *)arg;
    for (int i=0; i<s; ++i){
      if (search[i] == -1){
        printf("result of search #%d = -1 \n",(i+1));
      }
      else{
        printf("result of search #%d = %d \n",(i+1), fibonacci[search[i]]);
      }
    }
  return NULL;
}



int main()
{
        pthread_t thread1, thread2;
        printf("Enter the term of fibonacci sequence: \n");
        scanf("%d", &n);
        if (n<0 || n>40){
          printf("Invalid input. n must be between 0 to 40");
        }
        pthread_create(&thread1, NULL, fibonacci_sequence, NULL);
        pthread_join(thread1, NULL);
        printf("Fibonacci sequence:\n");
        for (int i = 0; i <=n; ++i){
          printf("a[%d] = %d\n",i, fibonacci[i]);
        }
        int a;
        printf("How many number you are willing to search: \n");
        scanf("%d", &s);
        int search[s];
        for (int j = 0; j < s; ++j){
          printf("Enter search: \n");
          scanf("%d", &a);
          if (a<0 || a>n){
            search[j] = -1;
          }
          else{
            search[j] = a;
          }
        }
        pthread_create(&thread2, NULL, fibonacci_search, search);
        pthread_join(thread2, NULL);

        free(fibonacci);
    return 0;
}

