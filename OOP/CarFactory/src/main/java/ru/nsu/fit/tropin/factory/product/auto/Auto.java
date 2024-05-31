package ru.nsu.fit.tropin.factory.product.auto;

import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;
import lombok.ToString;
import lombok.extern.log4j.Log4j;
import ru.nsu.fit.tropin.factory.product.Product;

@Getter
@Setter
@ToString
@Log4j
public class Auto extends Product {

    private final Accessory accessory;
    private final Body body;
    private final Motor motor;

    public Auto(@NonNull Accessory accessory, @NonNull Body body, @NonNull Motor motor){
        super();
        this.accessory = accessory;
        this.body = body;
        this.motor = motor;
        log.info("Auto<" + this.getID() + "> (Body: <" + body.getID() + ">, "
                + "Motor: <" + motor.getID() + "> ,Accessory:<" + accessory.getID() + ">)");
    }

}
