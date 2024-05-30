package ru.nsu.fit.tropin.factory.worker;

import lombok.extern.log4j.Log4j2;
import ru.nsu.fit.tropin.threadpool.PooledThread;

@Log4j2
public class Worker extends PooledThread {

    public static int globalID = 0;

    public Worker() {
        super.setID(globalID);
        globalID++;
        log.info("Worker:" + super.getID() + " was created");
    }
}
