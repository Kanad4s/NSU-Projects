package ru.nsu.fit.tropin.threadpool;

import lombok.extern.log4j.Log4j2;
import java.util.LinkedList;
import java.util.List;

//@Log4j2
@Log4j2
public class ThreadPool{
    private final List<PooledThread> threads = new LinkedList<>();
    private final List<Task> taskQueue = new LinkedList<>();

    public void addTask(Task task) {
        synchronized (taskQueue) {
            taskQueue.add(task);
            taskQueue.notify();
        }
    }

    public int getTaskQueueSize(){
        return taskQueue.size();
    }

    public void addThread(PooledThread taskThread) {
        taskThread.setTaskQueue(taskQueue);
        threads.add(taskThread);
    }

    public void start(){
        for (PooledThread availableThread : threads) {
            availableThread.start();
            log.info();
//            log.info("Thread " + availableThread.getName() +  " was creating");
        }
    }
}