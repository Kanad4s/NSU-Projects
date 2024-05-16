package Services.Factory;

import Services.Operations.CommandAnnotation;
import Services.Operations.Operation;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.lang.annotation.Annotation;
import java.util.Properties;

public class OperationsFactory {
    private Properties properties;
    private static final Logger LOGGER = Logger.getLogger (OperationsFactory.class.getName ());
    private Class cl;

    public OperationsFactory() throws IOException {
        properties = new Properties();
        try(InputStream inputStream = getClass().getClassLoader().getResourceAsStream("application1.properties")) {
            properties.load(inputStream);
        } catch(IOException io) {
            LOGGER.warn ("Constructor Operations factory " + io.getMessage ());
            throw new IOException("application.properties was not found" + io.getMessage ());
        }
    }

    public Operation createClass(String className) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
        try {
            cl = Class.forName(properties.getProperty(className.toUpperCase()));
        } catch (NullPointerException nullPointerException) {
            throw new NullPointerException ("This command wasn't found " + nullPointerException.getMessage ());
        }

        Annotation[] annotations = cl.getAnnotations();

        for (Annotation annotation : annotations) {
            System.out.println(annotation);
            if (annotation instanceof CommandAnnotation) {
                return (Operation) cl.newInstance();
            }
        }
        throw new ClassNotFoundException(className);
    }
}
