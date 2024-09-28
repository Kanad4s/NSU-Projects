package ru.nsu.fit.tropin.factory.runnable_tasks;

import org.junit.BeforeClass;
import org.junit.Test;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Auto;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;
import static org.assertj.core.api.Assertions.assertThat;

public class DealerTest{

    private final int dealerDelay = 5;
    private final Warehouse<Auto> warehouse = new Warehouse<>(5, Warehouse.AUTO_WAREHOUSE_NAME);
    private final Dealer dealer = new Dealer(warehouse);

    @Test
    public void testTestRun() throws InterruptedException {
        dealer.setDealerDelay(dealerDelay);
        Motor motor = new Motor();
        Accessory accessory = new Accessory();
        Body body = new Body();
        Auto auto = new Auto(accessory, body, motor);
        warehouse.put(auto);

        Thread thread = new Thread(dealer);
        thread.start();

        Thread.sleep(dealerDelay);

        Auto autoDelivered = dealer.getCurrentAuto();
        thread.interrupt();

        assertThat(autoDelivered).isEqualTo(auto);
    }

    @Test
    public void testGetDealerDelay() {
        dealer.setDealerDelay(dealerDelay);
        assertThat(dealer.getDealerDelay()).isEqualTo(dealerDelay);
    }

    @Test
    public void testSetDealerDelay() {
        dealer.setDealerDelay(10);
        assertThat(dealer.getDealerDelay()).isEqualTo(10);
    }
}