package lessons;

import Services.GreetingService;
import Services.GreetingServiceImpl;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Description;

@Configuration
public class LessonsConfiguration {
    @Bean
    @Description("greeting description")
    GreetingService greetingService() {
        return new GreetingServiceImpl();
    }
}
