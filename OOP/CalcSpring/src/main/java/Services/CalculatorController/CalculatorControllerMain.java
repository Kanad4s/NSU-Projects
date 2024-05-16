package Services.CalculatorController;

import Configurations.Configurations;
import Services.Factory.OperationsFactory;
import Services.Operations.Operation;
import Services.Parser.LineParser;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.stereotype.Component;

import org.apache.log4j.Logger;
import java.io.*;

public class CalculatorControllerMain implements CalculatorController{
//    public static final Logger LOGGER = Logger.getLogger(CalculatorController.class.getName());
//    private Reader _reader;
    private BufferedReader _br;
    private String _line;
    private LineParser _lineParser;

    //бегу по классам с аннтацией и сравниваю с прочитанным
    @Autowired
    private Operation[] _operation;


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

    public void test() throws IOException {
        while ((_line = _br.readLine()) != null) {
            System.out.println(_line);
        }
    }

    @Override
    public void launch() throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException {
//        LOGGER.debug("Launching CalculatorController");
        ApplicationContext context = new AnnotationConfigApplicationContext(Configurations.class);
        _lineParser = context.getBean(LineParser.class);
//        _lineParser = new LineParserSimple();
        OperationsFactory operationsFactory = new OperationsFactory();
        while ((_line = _br.readLine()) != null) {
            _lineParser.parse(_line);
            String operation1 = _lineParser.getOperation();
            //Operation operation = context.getBean(operation1, Operation.class);
            //System.out.println(operation.getClass().getSimpleName());
            //do operation
            System.out.println(operation1);
            Operation operation = operationsFactory.createClass(operation1);
            System.out.println(operation.getClass());
        }
    }


}
