package ru.nsu.fit.tropin.factory.worker;

import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.threadpool.Task;
import ru.nsu.fit.tropin.threadpool.ThreadPool;

@Log4j
public class WorkerThreadPool {
    private final ThreadPool threadPool = new ThreadPool();

    public WorkerThreadPool(int workerCount) {
        for (int i = 0; i < workerCount; i++) {
            Worker worker = new Worker();
            threadPool.addThread(worker);
        }
    }

    public int getCurrentQueueTasksSize()
    {
        return threadPool.getTaskQueueSize();
    }

    public void addTask(Task task) {
        threadPool.addTask(task);
    }

    public void startWork() {
        threadPool.start();
    }
}
