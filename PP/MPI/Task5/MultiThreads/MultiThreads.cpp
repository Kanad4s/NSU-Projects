#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>
#include <malloc.h>
#include <string.h>
//#include <cstdlib>

#define L 1000
#define LISTS_COUNT 500
#define TASK_COUNT 2000
#define MIN_TASKS_TO_SHARE 2

#define EXECUTOR_FINISHED_WORK -1
#define SENDING_TASKS 656
#define SENDING_TASK_COUNT 787
#define NO_TASKS_TO_SHARE -565

pthread_t threads[2];
pthread_mutex_t mutex;
int* tasks;

double SummaryDisbalance = 0;
int FinishedExecution = 0;

int nProcesses;
int ProcessRank;
int RemainingTasks;
int ExecutedTasks;
int AdditionalTasks;
double globalRes = 0;

void printTasks(int taskSet[]) {
    printf("Process: %d", ProcessRank);
    for (int i = 0; i < TASK_COUNT; i++) {
        printf("%d ", taskSet[i]);
    }
    printf("\n");
}

void initializeTaskSet(int taskSet[], int taskCount, int iterCounter) {
    for (int i = 0; i < taskCount; i++) {
        taskSet[i] = abs(50 - i % 100) * abs(ProcessRank - (iterCounter % nProcesses)) * L;
    }
}

void executeTaskSet(int* taskSet) {
    for (int i = 0; i < RemainingTasks; i++) {
        pthread_mutex_lock(&mutex);
        int weight = taskSet[i];
        pthread_mutex_unlock(&mutex);

        for (int j = 0; j < weight; j++) {
            globalRes += cos(0.001488);
        }

        ExecutedTasks++;
    }
    RemainingTasks = 0;
}

void* ExecutorStartRoutine(void* args) {
    tasks = new int[TASK_COUNT];
    double startTime, finishTime, iterationDuration, shortestIteration, longestIteration;

    for (int i = 0; i < LISTS_COUNT; i++) {
        startTime = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Iteration: %d, Initializing tasks.", i);
        ExecutedTasks = 0;
        RemainingTasks = TASK_COUNT;
        AdditionalTasks = 0;
        initializeTaskSet(tasks, TASK_COUNT, i);
        executeTaskSet(tasks);
        printf("Process %d executed tasks in %f\n", ProcessRank, MPI_Wtime() - startTime);
        printf("Requesting additional tasks\n");
        int threadResponse;
        for (int procIdx = 0; procIdx < nProcesses; procIdx++) {
            if (procIdx != ProcessRank) {
                printf("Process %d is asking %d for tasks\n", ProcessRank, procIdx);
                MPI_Send(&ProcessRank, 1, MPI_INT, procIdx, 888, MPI_COMM_WORLD);
                MPI_Recv(&threadResponse, 1, MPI_INT, procIdx, SENDING_TASK_COUNT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Process %d answered %d\n", procIdx, threadResponse);
                if (threadResponse != NO_TASKS_TO_SHARE) {
                    AdditionalTasks = threadResponse;
                    for (int i = 0; i < TASK_COUNT; i++) {
                        tasks[i] = 0;
                    }
                    //memset(tasks, 0, TASK_COUNT);
                    MPI_Recv(tasks, AdditionalTasks, MPI_INT, procIdx, SENDING_TASKS, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    pthread_mutex_lock(&mutex);
                    RemainingTasks = AdditionalTasks;
                    pthread_mutex_unlock(&mutex);
                    executeTaskSet(tasks);
                }
            }

        }
        finishTime = MPI_Wtime();
        iterationDuration = finishTime - startTime;
        MPI_Allreduce(&iterationDuration, &longestIteration, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        MPI_Allreduce(&iterationDuration, &shortestIteration, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Process %d executed %d tasks. Additional: %d\n", ProcessRank, ExecutedTasks, AdditionalTasks);
        printf("Time taken: %f\n", iterationDuration);
        SummaryDisbalance += (longestIteration - shortestIteration) / longestIteration;
        printf("Max tim difference: %f\n", longestIteration - shortestIteration);
        printf("Disbalance: %f%\n", ((longestIteration - shortestIteration) / longestIteration) * 100);
    }

    pthread_mutex_lock(&mutex);
    FinishedExecution = 1;
    pthread_mutex_unlock(&mutex);
    int signal = EXECUTOR_FINISHED_WORK;
    MPI_Send(&signal, 1, MPI_INT, ProcessRank, 888, MPI_COMM_WORLD);
    free(tasks);
    pthread_exit(NULL);
}

void* ReceiverStartRoutine(void* args) {
    int askingProcRank, answer, pendingMessage;
    MPI_Status status;
    MPI_Barrier(MPI_COMM_WORLD);
    while (!FinishedExecution) {
        MPI_Recv(&pendingMessage, 1, MPI_INT, MPI_ANY_SOURCE, 888, MPI_COMM_WORLD, &status);

        if (pendingMessage == EXECUTOR_FINISHED_WORK) {
            printf("Executor finished work on proc %d\n", ProcessRank);
        }
        askingProcRank = pendingMessage;
        pthread_mutex_lock(&mutex);
        printf("Process %d requested task: %d\n", askingProcRank, RemainingTasks);
        if (RemainingTasks >= MIN_TASKS_TO_SHARE) {
            answer = RemainingTasks / (nProcesses * 2);
            RemainingTasks = RemainingTasks / (nProcesses * 2);
            printf("Sharing %d tasks\n", answer);
            MPI_Send(&answer, 1, MPI_INT, askingProcRank, SENDING_TASK_COUNT, MPI_COMM_WORLD);
            MPI_Send(&tasks[TASK_COUNT - answer], answer, MPI_INT, askingProcRank, SENDING_TASKS, MPI_COMM_WORLD);
        }
        else {
            answer = NO_TASKS_TO_SHARE;
            MPI_Send(&answer, 1, MPI_INT, askingProcRank, SENDING_TASK_COUNT, MPI_COMM_WORLD);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    int provider;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provider);
    if (provider != MPI_THREAD_MULTIPLE) {
        MPI_Finalize();
        return -1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    pthread_mutex_init(&mutex, nullptr);
    pthread_attr_t threadAttributes;

    double start = MPI_Wtime();
    pthread_attr_init(&threadAttributes);
    pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &threadAttributes, ReceiverStartRoutine, NULL);
    pthread_create(&threads[1], &threadAttributes, ExecutorStartRoutine, NULL);
    pthread_join(threads[0], nullptr);
    pthread_join(threads[1], nullptr);
    pthread_attr_destroy(&threadAttributes);
    pthread_mutex_destroy(&mutex);
    double finish = MPI_Wtime();

    if (ProcessRank == 0) {
        printf("Summary disbalance: %f%\n", SummaryDisbalance / (LISTS_COUNT) * 100);
        printf("Time taken: %f\n", finish - start);
    }

    MPI_Finalize();
    return 0;
}