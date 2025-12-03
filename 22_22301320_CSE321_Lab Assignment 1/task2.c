#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>


#define BREAD 0
#define CHEESE 1
#define LETTUCE 2


int N;
int table[2];
int count = 0;
pthread_mutex_t table_lock;
sem_t empty_table;
sem_t makers_sem[3];

const char* ingredient_names[] = {"Bread", "Cheese", "Lettuce"};
const char* maker_names[] = {"Maker A", "Maker B", "Maker C"};


void *suppliers_func(void *arg){
   srand(time(NULL));
   for (int i = 0; i < N; i++){
       sem_wait(&empty_table);
       int ingredient1 = rand() % 3;
       int ingredient2 = rand() % 3;
       while (ingredient1 == ingredient2){
           ingredient2 = rand() % 3;
       }
       pthread_mutex_lock(&table_lock);
       table[0] = ingredient1;
       table[1] = ingredient2;
       pthread_mutex_unlock(&table_lock);
       printf("Supplier places: %s and %s\n", ingredient_names[ingredient1], ingredient_names[ingredient2]);
       int missing = 3 - ingredient1 - ingredient2;
       sem_post(&makers_sem[missing]);
   }


}


void *makers_func(void *arg){
   int maker_id = *((int*)arg);
   const char* maker_name = maker_names[maker_id];
   while (1){
       sem_wait(&makers_sem[maker_id]);
       pthread_mutex_lock(&table_lock);
       if (count >= N) {
           pthread_mutex_unlock(&table_lock);
           break;
       }
       count++;
       int ing1 = table[0];
       int ing2 = table[1];
       pthread_mutex_unlock(&table_lock);
       printf("%s picks up %s and %s\n", maker_name, ingredient_names[ing1], ingredient_names[ing2]);
       printf("%s is making the sandwich...\n", maker_name);
       usleep(500000);
       printf("%s finished making the sandwich and eats it\n", maker_name);
       printf("%s signals Supplier\n", maker_name);
       printf("\n");
       sem_post(&empty_table);
      
   }
   return NULL;
}



int main() {
   pthread_t suppliers_t, makers_t[3];
   int maker_ids[3] = {0, 1, 2};
   scanf("%d", &N);
   pthread_mutex_init(&table_lock, NULL);
   sem_init(&empty_table, 0, 1);


   for (int i = 0; i < 3; i++) {
       sem_init(&makers_sem[i], 0, 0);
   }
   pthread_create(&suppliers_t, NULL, suppliers_func, NULL);
    
   for (int i = 0; i < 3; i++) {
       pthread_create(&makers_t[i], NULL, makers_func, &maker_ids[i]);
   }
  
   pthread_join(suppliers_t, NULL);

   for (int i = 0; i < 3; i++) {
       sem_post(&makers_sem[i]);
   }
    
   for (int i = 0; i < 3; i++) {
       pthread_join(makers_t[i], NULL);
   }

   pthread_mutex_destroy(&table_lock);
   sem_destroy(&empty_table);
   for (int i = 0; i < 3; i++) {
       sem_destroy(&makers_sem[i]);
   }
  
  
   return 0;
}



