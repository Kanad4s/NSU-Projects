package ru.nsu.fit.tropin.Operations;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import ru.nsu.fit.tropin.CalculatorController.CalculatorStack;
import ru.nsu.fit.tropin.Configurations.Configurations;
import ru.nsu.fit.tropin.Exceptions.OperationException;

import java.util.ArrayList;
import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.*;


class PlusTest {
    private CalculatorStack calculatorStack;
    private Plus plus;
    private ArrayList<String> list;

    @BeforeEach
    void setUp() {
//        ApplicationContext context = new AnnotationConfigApplicationContext(Configurations.class);
//        calculatorStack = context.getBean(CalculatorStack.class);
        calculatorStack = new CalculatorStack();
        plus = new Plus();
        list = new ArrayList<>();
    }

    @Test
    void calculation() throws Exception {
        calculatorStack.pushValue("10");
        calculatorStack.pushValue("40");
        plus.calculation(calculatorStack, list);
        assertEquals(50, calculatorStack.popValue());
    }

    @Test
    void calculationEmpty() {
        assertThrows(EmptyStackException.class, () -> plus.calculation(calculatorStack, list));
    }

    @Test
    void calculationNotEnoughArgs() {
        calculatorStack.pushValue("10");
        assertThrows(EmptyStackException.class, () -> plus.calculation(calculatorStack, list));
    }

}