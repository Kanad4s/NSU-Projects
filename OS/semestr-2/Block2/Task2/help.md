## Синхронизация

А когда она нужна? т.к. это накладные расходы\
pthread_setaffinity_np - привязывает поток к ядру\
Выявление исправности синхронизации зависит от уровня оптимизации\
volatile - оптимизации вокруг этой переменной не будут применены компилятором

### 2 типа задач на синхронихацию
* производитель-потребитель
* обедающие философы
  
Мы работаем над первой, т.е. над какой-то структурой?\
Критическая секция - код, в котором данные консистентны 

### CAS - compare and swap
Атомарная операция сраванивает значение переменной с чем-то и заменяет его
```c
cas($flag, 0, 1) {
    if(*flag != 0) {
        return err;
    }
    *flag = 1;
}
```
т.е. переменная будет изменена только если такое значение она имеет

```bash
time ./prog
```

загрузка цп
```bash
htop
```

Сложение со спинлоком на разных ядрах дольше, чем на одном
т.к. конкуренции за лок не будет. На одном ядре каждый поток работает квант времени, соответсвенно потоки успеют больше раз сложить.\
На разных же ядрах каждый раз они постоянно лочатся

По хорошему, если лочка не получилась, то надо отдать поток(sched_yield()) и поток, в котором был захвачен спинлок, снова начнет работу\
Неправильно передавать управление другому потоку через sched_yield(), она по сути вызывает функцию планировщика schedule(), а планировщик может вновь отдать этот поток на выполнение\
Это работает и для разных ядр и для одного. С разными получится аналогия одного ядра по времени работы

Что значит вытеснили поток и как его вернуть?\
SpinLock по сути бесконечный цикл
```C
typedef struct {
    int lock;
} spinlock_t;

void spinlock_init(spinlock_t *s) {
    s->lock = 1;
}

void spinlock_lock(spinlock_t *s) {
    while(1) {
        int one = 1;
        if (atomic_compare_exchange_strong(&s->lock, &one, 0))
            break;
        // sched_yield();
    }
}

void spinlock_unlock(spinlock_t *s) {
    const int zero = 0;
    atomic_compare_exchange_strong(&s->lock, &zero, 1);
}
```

## Результаты
### Без синхронизации
* попыток add в разы больше, чем получилось примерно в 4 раза
* попыток get в разы больше, чем получилось
* загрузка на максимум
* возможность работать только из под одного ядра
* разница add и get постоянно растет
### SpinLock
* каждая попытка add удалась(на 1 ядре)
* примерно одинаковое количество add и get операций(на разных ядрах разница постоянная)
* далеко не каждая попытка get удалась
* загрузка на максимум
### SpinLock with usleep
* каждая попытка add удалась
* одинаковое количество add и get операций
* попыток get очень много
* загрузка цп writer ~7%
* загрузка цп reader 100%
* одинаковый результат в одно- и многоядерном режимах
### Mutex одноядерный
* попыток add и get больше, чем удалось
* много отработал либо add, либо get
* загрузка на максимум
### Mutex разноядерый
* каждая попытка add удалась
* попыток get очень много
* количество add и get операций несколько разнится 
* загрузка на максимум
### Condvar 
* каждая попытка get и add удалась
* нет зависимости от привязки к ядру
* загрузка ядер на ~40%
### Semaphore
* практически каждая попытка add удалась(на разных потоках все удались)
* разница получившихся add и get плавает сильно, но из крайних состояний сразу выходит(на разных ядрах разница минимальна)
* загрузка максимальна


futex индентифицируется по адресу переменной, что очевидно для потоков в общей памяти, а при работе с потоками в разных адрессных пространствах эта переменная &m->lock лежит в разделяемой памяти, поэтому процессор тоже понимает кому принадлежит это всё\
futex лежит на стеке, его не надо инициализировать
### аргумент после futex
* wakeup - сколько процессов разблокировать
* wait - значение которое нужно проверить у пременной &m->lock, чтоб в блокировку не встать

### Conditional variable
Это объект, на который можно сделать wait, signal, broadcast(сигнал для всех)  
wait - залочиться  
signal - разлочиться  
Но нужен флаг, который говорит, какой поток должен печатать. Тогда флаг - событие, на которое надо реагировать, а condvar - способ уведомить, что флаг поменялся. Mutex нужен для корректной работы с разделяемой переменной flag. Тогда wait разлочивает mutex.
```C
while() {
    lock(mutex);
    while (flag != 1) {
        wait(condvar, mutex);
    }
    work();
    flag = 2;
    signal(condvar);
    unlock(mutex);
}
```
Симметричный код на другом потоке.  

### RW Lock (Read-Write Lock)
Что-то на интуитивно понятном  
Доступ к секции может быть на R либо на W
Если много потоков R, то все ок, а если потоки на W, то надо поочереди  
Схема такая, потоки встают в 2 очереди: W и R - условно сначала работают все потоки на R, а потом на W, и т.д.

### Semaphore (семафор-счетчик)
Имеем 2 атомарные функции: wait(), post() и счетчик "a" до 100  
wait() - вычитает 1
post() - прибавляет 1
В чем прикол? если сделать wait, а на счетчике 0, то встаем в wait, пока кто-то не сделает post  
Зачем mutex, если есть semaphore
Главное отличие - mutex имеет владельца(пид потока/процесса), а у семафора его нет. Mutex нужен для формирования критических секций. Т.е. странно, что из другого потока в любое время можно было бы снять mutex. (сделать внутри туалета замок, который легко открыть снаружи)  
Семафоры можно использовать вместо мутекса, а наоборот - нельзя.  
Семафоры позволяют делать взаимоисключающие блокировки  
Похоже на condvar по реакции на событие

### Barrier
Хочу чтоб алгоритм в потоках начался одновременно, когда все сделают init()  
Таким образом, потоки встают в ожидание, когда доходят до барьера, и ждут остальные

спинлок медленнее, т.к. больше времени проводится в систайме
спинлок лучше в коротких захватах, а мутекс в длинных