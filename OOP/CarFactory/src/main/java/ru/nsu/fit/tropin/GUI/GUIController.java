package ru.nsu.fit.tropin.GUI;
import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.factory.AutoFactory;
import ru.nsu.fit.tropin.observer.Observer;

@Log4j
public class GUIController {

    private final AutoFactory autoFactory;

    public GUIController(){
        autoFactory = new AutoFactory();
    }

    public void startFactory() {
        autoFactory.start();
    }

    public void setBodySupplierDelay(int bodySupplierDelay) {
        autoFactory.setBodySupplierDelay(bodySupplierDelay);
    }

    public void setMotorSupplierDelay(int motorSupplierDelay) {
        autoFactory.setMotorSupplierDelay(motorSupplierDelay);
    }

    public void setAccessorySupplierDelay(int accessorySupplierDelay) {
        autoFactory.setAccessorySupplierDelay(accessorySupplierDelay);
    }

    public void setDealerDelay(int dealerDelay) {
        autoFactory.setDealerDelay(dealerDelay);
    }

    public void addBodyWarehouseObserver(Observer o){
        autoFactory.addBodyWarehouseObserver(o);
    }

    public void addMotorWarehouseObserver(Observer o){
        autoFactory.addMotorWarehouseObserver(o);
    }

    public void addAccessoryWarehouseObserver(Observer o){
        autoFactory.addAccessoryWarehouseObserver(o);
    }

    public void addAutoWarehouseObserver(Observer o){
        autoFactory.addAutoWarehouseObserver(o);
    }


}
