package ru.nsu.fit.tropin.factory.warehouse;

import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;
import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.factory.product.Product;
import ru.nsu.fit.tropin.observer.Observable;
import ru.nsu.fit.tropin.observer.Observer;

import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.Set;

@Getter
@Setter
@Log4j
public class Warehouse<T extends Product> implements Observable {

    public final static String ACCESSORY_WAREHOUSE_NAME = "Accessory warehouse";
    public final static String BODY_WAREHOUSE_NAME = "Body warehouse";
    public final static String MOTOR_WAREHOUSE_NAME = "Motor warehouse";
    public final static String AUTO_WAREHOUSE_NAME = "Auto warehouse";

    private final String warehouseName;
    private final ArrayDeque<T> products;
    private final int warehouseCapacity;

    @Getter private int totalProductCount = 0;
    private final Object monitor = new Object();

    private Set<Observer> observers = new HashSet<>();

    public Warehouse(int warehouseCapacity, String warehouseName) {
        this.products = new ArrayDeque<>();
        this.warehouseCapacity = warehouseCapacity;
        this.warehouseName = warehouseName;
        log.info(warehouseName + " was create, capacity:"+ warehouseCapacity);
    }

    public void put (@NonNull T newItem) throws InterruptedException {
        synchronized (monitor) {
            while (products.size() >= warehouseCapacity && !Thread.currentThread().isInterrupted()) {
                try {
                    log.info(warehouseName + " is full");
                    monitor.wait();
                } catch (InterruptedException e) {
                    log.info(warehouseName + " is interrupted");
                    throw e;
                }
            }
            log.info(warehouseName + " got new product " + newItem.getClass().getName() + " ID: " + newItem.getID());
            products.add(newItem);
            log.info(warehouseName + " current size is " + products.size());

            totalProductCount++;
            notifyObservers();

            monitor.notify();
        }
    }

    public T deliver () throws InterruptedException {
        synchronized (monitor) {
            while (true) {
                try {
                    if (!products.isEmpty()) {
                        T item = products.getFirst();
                        products.remove();
                        log.info(warehouseName + " current size is " + products.size());
                        notifyObservers();
                        monitor.notifyAll();
                        log.info(warehouseName + " delivered ");
                        return item;
                    } else {
                        monitor.wait();
                    }
                } catch (InterruptedException e) {
                    log.info(warehouseName + " is interrupted");
                    throw e;
                }
            }
        }
    }


    @Override
    public void registerObserver(Observer o) {
        observers.add(o);
    }

    @Override
    public void removeObserver(Observer o) {
        observers.remove(o);
    }

    @Override
    public void notifyObservers() {
        for (Observer observer : observers)
            observer.update(products.size(), totalProductCount);
    }
}
