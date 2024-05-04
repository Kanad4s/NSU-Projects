package Lesson.Starter;

import Lesson.LessonsConfiguration;
import Services.GreetingService;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.logging.Logger;

public class Starter {
    private static final Logger logger = Logger.getLogger(Starter.class.getName());

    public static void main1(String[] args) {
        logger.info("Starting configuration...");
        ApplicationContext context = new AnnotationConfigApplicationContext(LessonsConfiguration.class);
        GreetingService greetingService = context.getBean(GreetingService.class);
        logger.info(greetingService.sayGreeting());
    }
}
