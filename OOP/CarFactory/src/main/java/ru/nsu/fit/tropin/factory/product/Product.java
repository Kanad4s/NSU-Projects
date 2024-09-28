package ru.nsu.fit.tropin.factory.product;

import lombok.Getter;

@Getter
public abstract class Product {

    public static int globalID = 0;
    private int ID;
    private final Object monitor = new Object();

    public Product(){
        synchronized (monitor){
            ID = globalID;
            globalID++;
        }
    }
}
