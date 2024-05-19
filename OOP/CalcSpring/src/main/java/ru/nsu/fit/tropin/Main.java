package ru.nsu.fit.tropin;

import ru.nsu.fit.tropin.Configurations.Configurations;
import ru.nsu.fit.tropin.CalculatorController.CalculatorController;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import org.apache.log4j.Logger;


@SpringBootApplication
public class Main {
    private static final Logger LOGGER = Logger.getLogger(Main.class.getName());

	public static void main(String[] args) throws Exception {
        ApplicationContext context = new AnnotationConfigApplicationContext(Configurations.class);
        CalculatorController controller = context.getBean("calculatorControllerMain", CalculatorController.class);
        LOGGER.info("Set input");
        if (args.length == 0) {
            controller.setInput(System.in);
        } else {
            controller.setInput(args[0]);
        }
        controller.launch();
	}
}
