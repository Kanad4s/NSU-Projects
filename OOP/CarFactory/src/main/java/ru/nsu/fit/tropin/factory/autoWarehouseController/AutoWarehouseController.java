package ru.nsu.fit.tropin.factory.autoWarehouseController;

import lombok.extern.log4j.Log4j2;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;
import ru.nsu.fit.tropin.factory.worker.BuildCar;
import ru.nsu.fit.tropin.factory.worker.WorkerThreadPool;
import ru.nsu.fit.tropin.observer.Observer;

@Log4j2
public class AutoWarehouseController implements Observer {

    private final Warehouse<Body> bodyWarehouse;
    private final Warehouse<Accessory> accessoryWarehouse;
    private final Warehouse<Motor> motorWarehouse;
    private final Warehouse<Auto> autoWarehouse;
    private final WorkerThreadPool workerThreadPool;

    public AutoWarehouseController(Warehouse<Body> bodyWarehouse,
                                   Warehouse<Accessory> accessoryWarehouse,
                                   Warehouse<Motor> motorWarehouse,
                                   Warehouse<Auto> autoWarehouse,
                                   WorkerThreadPool workerThreadPool) {

        log.info("AutoWarehouseController is creating...");
        this.bodyWarehouse = bodyWarehouse;
        this.accessoryWarehouse = accessoryWarehouse;
        this.motorWarehouse = motorWarehouse;
        this.autoWarehouse = autoWarehouse;
        this.workerThreadPool = workerThreadPool;

        autoWarehouse.registerObserver(this);
        for (int i = 0; i < autoWarehouse.getWarehouseCapacity(); i++) {
            createAuto();
        }
    }

    public void createAuto(){
        workerThreadPool.addTask(new BuildCar(accessoryWarehouse, motorWarehouse,
                bodyWarehouse, autoWarehouse));
    }
    @Override
    public void update(int currentProductCount, int totalProductCount) {

        log.info("Data is changed, there is a request to create new machines: current product count = " +
                currentProductCount + " " + " total product count = " + totalProductCount);
        int currentQueueTasksSize = workerThreadPool.getCurrentQueueTasksSize();

        if (currentProductCount + currentQueueTasksSize < autoWarehouse.getWarehouseCapacity()) {
            for (int i = 0; i < autoWarehouse.getWarehouseCapacity() - currentProductCount + currentQueueTasksSize; i++) {
                createAuto();
            }
        }
    }
}
