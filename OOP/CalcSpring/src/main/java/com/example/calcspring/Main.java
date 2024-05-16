package com.example.calcspring;

import Configurations.Configurations;
import Lesson.LessonsConfiguration;
import Services.CalculatorController.CalculatorController;
import Services.GreetingService;
import Services.Operations.Define;
import Services.Operations.Operation;
import Services.Operations.Push;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.io.IOException;
import org.apache.log4j.Logger;


@SpringBootApplication
public class Main {
    private static final Logger LOGGER = Logger.getLogger(Main.class.getName());

	public static void main(String[] args) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException {
        ApplicationContext context = new AnnotationConfigApplicationContext(Configurations.class);
        CalculatorController controller = context.getBean(CalculatorController.class);
        if (args.length == 0) {
            controller.setInput(System.in);
        } else {
            controller.setInput(args[0]);
        }
        //controller.test();
        controller.launch();
        Operation operation = new Push();
        System.out.println(operation.getClass());
        System.out.println(operation.getClass().getSimpleName().toLowerCase());
        operation = new Define();

        System.out.println(operation.getClass());
        System.out.println(operation.getClass().getSimpleName().toUpperCase());
		//SpringApplication.run(CalcSpringApplication.class, args);
        LOGGER.info("Starting my configuration...");
        ApplicationContext context1 = new AnnotationConfigApplicationContext(LessonsConfiguration.class);
        GreetingService greetingService = context1.getBean(GreetingService.class);
        LOGGER.info(greetingService.sayGreeting());
	}

}
