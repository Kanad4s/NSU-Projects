package ru.nsu.fit.tropin.factory.config_parser;

import java.io.InputStream;
import java.util.Properties;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class ConfigParser {

    public final static String DEFAULT_VALUE = "0";
    private static String CONFIG_PROPERTIES_FILE_NAME = "config.properties";
    private static Properties configProperties;

    public ConfigParser(){}
    public ConfigParser(String fileName){
        CONFIG_PROPERTIES_FILE_NAME = fileName;
    }

    public void readProperties(){
        log.info("Reading config file...");

        try (InputStream configStream = ConfigParser.class.getClassLoader()
                .getResourceAsStream(CONFIG_PROPERTIES_FILE_NAME)) {

            if (configStream == null) {
                throw new NullPointerException("not such file");
            }
            configProperties = new Properties();
            configProperties.load(configStream);

        } catch (Exception e) {
            log.error(e.getMessage());
        }
    }

    public int getSupplierDelay(){
        int supplierDelay = Integer.parseInt(configProperties.getProperty("SupplierDelay", DEFAULT_VALUE));
        return supplierDelay;
    }

    public int getDealerDelay(){
        int dealerDelay = Integer.parseInt(configProperties.getProperty("DealerDelay", DEFAULT_VALUE));
        return dealerDelay;
    }

    public int getWarehouseBodyCapacity(){
        int warehouseBodyCapacity = Integer.parseInt(configProperties.getProperty("WarehouseBodyCapacity", DEFAULT_VALUE));
        return warehouseBodyCapacity;
    }

    public int getWarehouseMotorCapacity(){
        int warehouseMotorCapacity = Integer.parseInt(configProperties.getProperty("WarehouseMotorCapacity", DEFAULT_VALUE));
        return warehouseMotorCapacity;
    }

    public int getWarehouseAccessoryCapacity(){
        int warehouseAccessoryCapacity = Integer.parseInt(configProperties.getProperty("WarehouseAccessoryCapacity", DEFAULT_VALUE));
        return warehouseAccessoryCapacity;
    }

    public int getWarehouseAutoCapacity(){
        int warehouseAutoCapacity = Integer.parseInt(configProperties.getProperty("WarehouseAutoCapacity", DEFAULT_VALUE));
        return warehouseAutoCapacity;
    }

    public int getSupplierCount(){
        int supplierCount = Integer.parseInt(configProperties.getProperty("SuppliersCount", DEFAULT_VALUE));
        return supplierCount;
    }

    public int getWorkerCount(){
        int workerCount = Integer.parseInt(configProperties.getProperty("WorkersCount", DEFAULT_VALUE));
        return workerCount;
    }

    public int getDealerCount(){
        int dealerCount = Integer.parseInt(configProperties.getProperty("DealersCount", DEFAULT_VALUE));
        return dealerCount;
    }

    public boolean getLogSale(){
        boolean logSaleStatus = Boolean.parseBoolean(configProperties.getProperty("LogSale", "true"));
        return logSaleStatus;
    }

}