package ru.nsu.fit.tropin.CalculatorController;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.stereotype.Component;
import ru.nsu.fit.tropin.Configurations.Configurations;
import ru.nsu.fit.tropin.Operations.WrongOperation;
import ru.nsu.fit.tropin.Services.Factory.OperationsFactory;
import ru.nsu.fit.tropin.Operations.Operation;
import ru.nsu.fit.tropin.Services.Parser.LineParser;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.io.*;

@Component
public class CalculatorControllerMain implements CalculatorController{
//    public static final Logger LOGGER = Logger.getLogger(CalculatorController.class.getName());//бегу по классам с аннтацией и сравниваю с прочитанным
    @Autowired
    private Operation[] _operations;
    @Autowired
    private LineParser _lineParser;
    private BufferedReader _br;
    private String _line;
    @Autowired
    private CalculatorStack _calculatorStack;

    public CalculatorControllerMain() {}

    @Override
    public void setInput(InputStream inputStream) {
//        LOGGER.debug("Set console input stream");
        _br = new BufferedReader(new InputStreamReader(System.in));
    }

    @Override
    public void setInput(String fileName) throws FileNotFoundException {
//        LOGGER.debug("Set file input stream");
        _br = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)));
    }

    @Override
    public void test() throws IOException {
        for (int i = 0; i < _operations.length; i++) {
            System.out.println(_operations[i].getClass());
        }
    }

    @Override
    public void launch() throws Exception {
//        LOGGER.debug("Launching CalculatorController");
        while ((_line = _br.readLine()) != null) {
            _lineParser.parse(_line);
            String operationName = _lineParser.getOperation();
            System.out.println("Operation:" + operationName + "\n");
            Operation operation = chooseOperation(operationName);
            System.out.println(operation.getClass());
            operation.calculation(_calculatorStack, _lineParser.getParameters());
        }
    }

    private Operation chooseOperation(String name) {
        for (int i = 0; i < _operations.length; i++) {
            if (_operations[i].getClass().getSimpleName().toUpperCase().equals(name)) {
                return _operations[i];
            }
        }
        return new WrongOperation();
    }
}
