package ru.nsu.fit.tropin.factory;

import java.util.ArrayList;
import java.util.List;

import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.factory.autoWarehouseController.AutoWarehouseController;
import ru.nsu.fit.tropin.factory.config_parser.ConfigParser;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.runnable_tasks.Dealer;
import ru.nsu.fit.tropin.factory.runnable_tasks.Supplier;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;
import ru.nsu.fit.tropin.factory.worker.WorkerThreadPool;
import ru.nsu.fit.tropin.observer.Observer;

@Log4j
public class AutoFactory {

    private final Warehouse<Accessory> accessoryWarehouse;
    private final Warehouse<Body> bodyWarehouse;
    private final Warehouse<Motor> motorWarehouse;
    private final Warehouse<Auto> autoWarehouse;

    private final Supplier<Body> bodySupplier;
    private final Supplier<Motor> motorSupplier;
    private final Supplier<Accessory> accessorySupplier;

    private final WorkerThreadPool workerThreadPool;
    private final List<Dealer> dealers;

    private final Thread motorSupplierThread;
    private final Thread accessorySupplierThread;
    private final Thread bodySupplierThread;

    private final AutoWarehouseController autoWarehouseController;


    public AutoFactory() {
        ConfigParser configParser = new ConfigParser();
        configParser.readProperties();

        log.info("Creating warehouses...");
        accessoryWarehouse = new Warehouse<>(configParser.getWarehouseAccessoryCapacity(),
                Warehouse.ACCESSORY_WAREHOUSE_NAME);
        bodyWarehouse = new Warehouse<>(configParser.getWarehouseAccessoryCapacity(),
                Warehouse.BODY_WAREHOUSE_NAME);
        motorWarehouse = new Warehouse<>(configParser.getWarehouseAccessoryCapacity(),
                Warehouse.MOTOR_WAREHOUSE_NAME);
        autoWarehouse = new Warehouse<>(configParser.getWarehouseAccessoryCapacity(),
                Warehouse.AUTO_WAREHOUSE_NAME);

        log.info("Creating suppliers...");

        motorSupplier = new Supplier<>(motorWarehouse, Motor.class);
        motorSupplier.setSupplierDelay(configParser.getSupplierDelay());

        bodySupplier = new Supplier<>(bodyWarehouse, Body.class);
        bodySupplier.setSupplierDelay(configParser.getSupplierDelay());

        accessorySupplier = new Supplier<>(accessoryWarehouse, Accessory.class);
        accessorySupplier.setSupplierDelay(configParser.getSupplierDelay());

        log.info("Creating worker threadpool...");
        workerThreadPool = new WorkerThreadPool(configParser.getWorkerCount());

        log.info("Creating dealers..");
        dealers = new ArrayList<>();
        for (int i = 0; i < configParser.getDealerCount(); ++i) {
            Dealer dealer = new Dealer(autoWarehouse);
            dealer.setDealerDelay(configParser.getDealerDelay());
            dealers.add(dealer);
        }

        log.info("Creating threads..");
        motorSupplierThread = new Thread(motorSupplier);
        accessorySupplierThread = new Thread(accessorySupplier);
        bodySupplierThread = new Thread(bodySupplier);

        log.info("Creating auto warehouse controller..");
        autoWarehouseController = new AutoWarehouseController(bodyWarehouse, accessoryWarehouse,
                                                              motorWarehouse, autoWarehouse, workerThreadPool);
    }

    public void start() {
        log.info("Starting factory...");

        motorSupplierThread.start();
        bodySupplierThread.start();
        accessorySupplierThread.start();

        workerThreadPool.startWork();

        for (Dealer dealer : dealers) {
            Thread dealerThread = new Thread(dealer);
            dealerThread.start();
        }
    }

    public void addBodyWarehouseObserver(Observer observer){
        bodyWarehouse.registerObserver(observer);
    }

    public void addMotorWarehouseObserver(Observer observer){
        motorWarehouse.registerObserver(observer);
    }

    public void addAccessoryWarehouseObserver(Observer observer){
        accessoryWarehouse.registerObserver(observer);
    }

    public void addAutoWarehouseObserver(Observer observer){
        autoWarehouse.registerObserver(observer);
    }

    public void setMotorSupplierDelay(int delay){
        motorSupplier.setSupplierDelay(delay);
    }

    public void setBodySupplierDelay(int delay){
        bodySupplier.setSupplierDelay(delay);
    }

    public void setAccessorySupplierDelay(int delay){
        accessorySupplier.setSupplierDelay(delay);
    }

    public void setDealerDelay(int delay){
        for (Dealer dealer : dealers) {
            dealer.setDealerDelay(delay);
        }
    }
}
