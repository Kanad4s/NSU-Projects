package ru.nsu.fit.tropin.threadpool;

public interface Task {
    void exec() throws InterruptedException;
}