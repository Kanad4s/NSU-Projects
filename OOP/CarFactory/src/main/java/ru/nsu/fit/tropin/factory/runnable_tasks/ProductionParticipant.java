package ru.nsu.fit.tropin.factory.runnable_tasks;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class ProductionParticipant{

    private int delay;
    public final static int MAX_DELAY = 31000;
    public final static int MIN_DELAY = 1000;

    public void setDelay(int delay){
        if (delay < MIN_DELAY || delay > MAX_DELAY){
            log.error("can't change delay");
            throw new IllegalArgumentException(delay + "is illegal argument");
        }
        this.delay = delay;
        log.info("New delay = " + delay);
    }

    public int getDelay(){
        return delay;
    }

}
