package ru.nsu.fit.tropin.CalculatorController;

import org.apache.log4j.Logger;
import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.Operations.WrongOperation;
import ru.nsu.fit.tropin.Operations.Operation;
import ru.nsu.fit.tropin.Services.Parser.LineParser.LineParser;
import org.springframework.beans.factory.annotation.Autowired;

import java.io.*;

//бегу по классам с аннотацией и сравниваю с прочитанным
@Component
public class CalculatorControllerMain implements CalculatorController{
    public static final Logger LOGGER = Logger.getLogger(CalculatorController.class.getName());
    @Autowired
    private Operation[] _operations;
    @Autowired
    private LineParser _lineParser;
    @Autowired
    private CalculatorStack _calculatorStack;
    private BufferedReader _br;
    private String _line;

    public CalculatorControllerMain() {}

    @Override
    public void setInput(InputStreamReader inputStreamReader) {
        LOGGER.debug("Set reader input stream");
        _br = new BufferedReader(inputStreamReader);
    }

    public void test() throws IOException {
        for (Operation operation : _operations) {
            System.out.println(operation.getClass());
        }
    }

    @Override
    public void launch() throws Exception {
        LOGGER.debug("Launching CalculatorController");
        while ((_line = _br.readLine()) != null) {
            _lineParser.parse(_line);
            String operationName = _lineParser.getOperation();
//            System.out.println("Operation:" + operationName + "\n");
            Operation operation = chooseOperation(operationName);
//            System.out.println(operation.getClass());
            operation.calculation(_calculatorStack, _lineParser.getParameters());
        }
    }

    private Operation chooseOperation(String name) {
        LOGGER.debug("Choose operation");
        for (Operation operation : _operations) {
            if (operation.getClass().getSimpleName().toUpperCase().equals(name)) {
                return operation;
            }
        }
        return new WrongOperation();
    }
}
