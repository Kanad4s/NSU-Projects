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

import java.io.*;

public class CalculatorControllerMain implements CalculatorController{
//    private Reader _reader;
    private BufferedReader _br;
    private String _line;
    private LineParser _lineParser;

    public CalculatorControllerMain() {}

    @Override
    public void setInput(InputStream inputStream) {
        _br = new BufferedReader(new InputStreamReader(System.in));
    }

    @Override
    public void setInput(String fileName) throws FileNotFoundException {
        _br = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)));
    }

    @Override
    public void test() throws IOException {
        while ((_line = _br.readLine()) != null) {
            System.out.println(_line);
        }
    }

    @Override
    public void launch() throws IOException, ClassNotFoundException {
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
            Operation operation = operationsFactory.createClass(operation1);
            System.out.println(operation.getClass());
        }
    }


}
