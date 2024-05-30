package ru.nsu.fit.tropin.factory.runnable_tasks;

import lombok.extern.log4j.Log4j2;
import ru.nsu.fit.tropin.factory.product.Product;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;

import java.lang.reflect.InvocationTargetException;

@Log4j2

public class Supplier<T extends Product>  extends ProductionParticipant implements Runnable{

    private final Warehouse<T> warehouse;
    private final Class<T> productClass;

    public Supplier(Warehouse<T> warehouse, Class<T> productClass) {
        this.warehouse = warehouse;
        this.productClass = productClass;
        log.info("Supplier for " + warehouse.getWarehouseName() + " was created");
    }

    @Override
    public void run() {
        while (true){
            try {
                Thread.sleep(super.getDelay());
                T product = productClass.getDeclaredConstructor().newInstance();
                warehouse.put(product);
            } catch (InterruptedException e) {
                log.info(Thread.currentThread().getName() + " was interrupted");
                break;
            }
            catch (InvocationTargetException | InstantiationException | IllegalAccessException | NoSuchMethodException e) {
                log.error(e.getMessage());
            }
        }
    }

    public int getSupplierDelay(){
        return super.getDelay();
    }
    public void setSupplierDelay(int delay){
        super.setDelay(delay);
    }
}
