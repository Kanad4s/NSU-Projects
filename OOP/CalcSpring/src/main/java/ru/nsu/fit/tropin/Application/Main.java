package ru.nsu.fit.tropin.Application;

import ru.nsu.fit.tropin.Configurations.Configurations;
import ru.nsu.fit.tropin.CalculatorController.CalculatorController;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import org.apache.log4j.Logger;
import ru.nsu.fit.tropin.Services.Parser.CmdParser.CmdParser;


@SpringBootApplication
public class Main {
    private static final Logger LOGGER = Logger.getLogger(Main.class.getName());

	public static void main(String[] args) throws Exception {
        ApplicationContext context = new AnnotationConfigApplicationContext(Configurations.class);
        CalculatorController controller = context.getBean("calculatorControllerMain", CalculatorController.class);
        CmdParser cmdParser = context.getBean(CmdParser.class);
        LOGGER.info("Parse args");
        if (cmdParser.parse(args)) {
            controller.setInput(cmdParser.getInputStream());
            controller.launch();
        }
        System.exit(0);
	}
}
