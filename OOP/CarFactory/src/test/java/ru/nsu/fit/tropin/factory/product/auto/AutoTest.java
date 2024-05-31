package ru.nsu.fit.tropin.factory.product.auto;

import org.junit.Test;

import static org.assertj.core.api.Assertions.assertThat;

public class AutoTest {

    @Test
    public void testTestToString() {
        Accessory accessory = new Accessory();
        Body body = new Body();
        Motor motor = new Motor();
        Auto auto = new Auto(accessory, body, motor);

        assertThat(accessory.getID()).isEqualTo(0);
        assertThat(body.getID()).isEqualTo(1);
        assertThat(motor.getID()).isEqualTo(2);
        assertThat(auto.getID()).isEqualTo(3);
    }
}