package ru.nsu.fit.tropin.threadpool;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j;
import lombok.extern.log4j.Log4j2;

import java.util.List;

@Log4j2
@Setter
@Getter
public class PooledThread extends Thread {
     public List<Task> taskQueue;
     private int ID;

     @Override
    public void run() {
        Task currentTask;
        while (!isInterrupted()) {
            synchronized (taskQueue) {
                if (taskQueue.isEmpty()) {
                    try {
                        taskQueue.wait();
                        log.info("Thread " + ID + " has no tasks");
                    }
                    catch (InterruptedException e) {
                        log.warn(taskQueue.get(0) + " was interrupted");
                    }
                    continue;
                }
                else {
                    currentTask = taskQueue.remove(0);
                    log.info("Task"  + ID + " get task");
                }
            }
            try {
                currentTask.exec();
            } catch (InterruptedException e) {
                log.warn(e.getMessage());
            }
        }
    }
}