package ru.nsu.fit.tropin.factory;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import ru.nsu.fit.tropin.factory.product.Product;
import ru.nsu.fit.tropin.factory.product.auto.Accessory;
import ru.nsu.fit.tropin.factory.product.auto.Body;
import ru.nsu.fit.tropin.factory.product.auto.Motor;
import ru.nsu.fit.tropin.factory.warehouse.Warehouse;

import static org.assertj.core.api.Assertions.assertThat;

import java.util.stream.Stream;

public class WarehouseTest {

    private static final int accessoryCapacity = 4;
    private static final int bodyCapacity = 5;
    private static final int motorCapacity = 6;


    @ParameterizedTest
    @MethodSource("generateWarehouseData")
    public void testPut(Warehouse<Product> warehouse, Product product) throws InterruptedException {
        warehouse.put(product);
        assertThat(warehouse.getProducts().getFirst()).isEqualTo(product);
        assertThat(warehouse.getProducts().size()).isEqualTo(1);

        if (Accessory.class.equals(product.getClass())) {
            assertThat(warehouse.getWarehouseCapacity()).isEqualTo(accessoryCapacity);
            assertThat(warehouse.getWarehouseName()).isEqualTo("Accessory warehouse");

        } else if (Motor.class.equals(product.getClass())) {
            assertThat(warehouse.getWarehouseCapacity()).isEqualTo(motorCapacity);
            assertThat(warehouse.getWarehouseName()).isEqualTo("Motor warehouse");
        }
        else{
            assertThat(warehouse.getWarehouseCapacity()).isEqualTo(bodyCapacity);
            assertThat(warehouse.getWarehouseName()).isEqualTo("Body warehouse");
        }

    }

    @ParameterizedTest
    @MethodSource("generateWarehouseData")
    public void testDeliver(Warehouse<Product> warehouse, Product product) throws InterruptedException {

        warehouse.put(product);
        Product product1 = warehouse.deliver();

        assertThat(warehouse.getProducts().size()).isEqualTo(0);
        assertThat(product1).isNotNull();
    }

    private static Stream<Arguments> generateWarehouseData()
    {
        return Stream.of(
                Arguments.of(new Warehouse<>(accessoryCapacity, Warehouse.ACCESSORY_WAREHOUSE_NAME), new Accessory()),
                Arguments.of(new Warehouse<>(bodyCapacity, Warehouse.BODY_WAREHOUSE_NAME), new Body()),
                Arguments.of(new Warehouse<>(motorCapacity, Warehouse.MOTOR_WAREHOUSE_NAME), new Motor())

        );
    }
}