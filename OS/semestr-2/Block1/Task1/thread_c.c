#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

// c)Впоточной функции распечатайте:
//  - идентификаторы процесса, родительского процесса, потока. Для 
//  получения идентификатора потока используйте функции pthread_self() и
//  gettid(). Сравните с тем что функция pthread_create() вернула через
//  первый аргумент. Объясните результат. Почему для сравнения
//  идентификаторов POSIX потоков надо использовать функцию
//  pthread_equal()?
//  -
//  адреса локальной, локальной статической, локальной константной и
//  глобальной переменных. Объясните результат.

// watch -d -n1 cat /proc//maps

static int global_value = 10;

void* my_thread(void *arg) {
   const int const_local = 5;
   static int static_local = 10;
   int local = 15;

   printf("my_thread [pid: %d, ppid: %d, tid: %d, pthread_self: %lu]: Hello from my_thread!\n", 
            getpid(), getppid(), gettid(), (unsigned long)pthread_self());
   // printf("my_thread: address local: %p, static_local: %p, const_local: %p, global_value: %p\n", 
   //          &local, &static_local, &const_local, &global_value );
   return NULL;
}

int main() {
   printf("main [pid: %d, ppid: %d, tid: %d]: Hello from main!\n", getpid(), getppid(), gettid());
//    sleep(10);
   const int const_local = 5;
   static int static_local = 10;
   int local = 15;
   // printf("main: address local: %p, static_local: %p, const_local: %p, global_value: %p\n", 
   //          &local, &static_local, &const_local, &global_value );

   pthread_t tid;
   int err = pthread_create(&tid, NULL, my_thread, NULL);
   printf("tif from main: %ld\n", tid);
   if (err) {
       fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err));
       return EXIT_FAILURE;
   }

   void* ret_val;
   err = pthread_join(tid, &ret_val);
   if (err) {
       fprintf(stderr, "main: pthread_join() failed %s\n", strerror(err));
       return EXIT_FAILURE;
   }

   // sleep(5);
   return EXIT_SUCCESS;
}