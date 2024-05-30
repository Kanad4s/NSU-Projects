package ru.nsu.fit.tropin.factory.runnable_tasks;

import lombok.*;
import lombok.extern.log4j.Log4j2;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;

@Log4j2
public class Dealer extends ProductionParticipant implements Runnable{

    private final Warehouse<Auto> autoWarehouse;
    @Getter
    private Auto currentAuto;

    public Dealer(Warehouse<Auto> autoWarehouse){
        this.autoWarehouse = autoWarehouse;
        log.info("Dealer was creates");
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(super.getDelay());
                currentAuto = autoWarehouse.deliver();
                log.info("Dealer bought : Auto<" + currentAuto.getID() + "> " + "(Body:<" + currentAuto.getBody().getID() + ">, " + "Motor:<" + currentAuto.getMotor().getID() + ">, " + "Accessory:<" + currentAuto.getAccessory().getID() + ">)");
            }
            catch (InterruptedException e){
                log.warn(e.getMessage());
            }

        }

    }

    public int getDealerDelay(){
        return super.getDelay();
    }
    public void setDealerDelay(int delay){
        super.setDelay(delay);
    }
}
