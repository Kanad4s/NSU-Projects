package ru.nsu.fit.tropin.factory.config_parser;

import junit.framework.TestCase;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static org.assertj.core.api.Assertions.assertThat;

public class ConfigParserTest extends TestCase {

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetSupplierDelay(ConfigParser configParser) {
        assertThat(configParser.getSupplierDelay()).isEqualTo(3000);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetDealerDelay(ConfigParser configParser) {
        assertThat(configParser.getDealerDelay()).isEqualTo(1000);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetWarehouseBodyCapacity(ConfigParser configParser) {
        assertThat(configParser.getWarehouseBodyCapacity()).isEqualTo(100);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetWarehouseMotorCapacity(ConfigParser configParser) {
        assertThat(configParser.getWarehouseMotorCapacity()).isEqualTo(120);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetWarehouseAccessoryCapacity(ConfigParser configParser) {
        assertThat(configParser.getWarehouseAccessoryCapacity()).isEqualTo(130);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetWarehouseAutoCapacity(ConfigParser configParser) {
        assertThat(configParser.getWarehouseAutoCapacity()).isEqualTo(140);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetSupplierCount(ConfigParser configParser) {
        assertThat(configParser.getSupplierCount()).isEqualTo(5);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetWorkerCount(ConfigParser configParser) {
        assertThat(configParser.getWorkerCount()).isEqualTo(10);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetDealerCount(ConfigParser configParser) {
        assertThat(configParser.getDealerCount()).isEqualTo(20);
    }

    @ParameterizedTest
    @MethodSource("generateConfigParser")
    public void testGetLogSale(ConfigParser configParser) {
        assertThat(configParser.getLogSale()).isTrue();
    }

    private static Stream<Arguments> generateConfigParser()
    {
        ConfigParser configParser = new ConfigParser("config_test.properties");
        configParser.readProperties();

        return Stream.of(
                Arguments.of(configParser)
        );
    }
}