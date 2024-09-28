package ru.nsu.fit.tropin.factory.worker;

import lombok.RequiredArgsConstructor;
import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;
import ru.nsu.fit.tropin.threadpool.Task;

@RequiredArgsConstructor
@Log4j
public class BuildCar implements Task {
    private final Warehouse<Accessory> accessoryWarehouse;
    private final Warehouse<Motor> motorWarehouse;
    private final Warehouse<Body> bodyWarehouse;
    private final Warehouse<Auto> autoWarehouse;

    @Override
    public void exec() throws InterruptedException {
        Accessory accessory= accessoryWarehouse.deliver();
        Body body = bodyWarehouse.deliver();
        Motor motor = motorWarehouse.deliver();

        Auto auto = new Auto(accessory, body, motor);
        autoWarehouse.put(auto);

        log.info(Thread.currentThread() + " build");
    }
}
