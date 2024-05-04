package Services.Factory;

import Services.Operations.Operation;

import java.lang.annotation.Annotation;

public class OperationsFactory {
    private Class cl;
    public Operation createClass(String className) throws ClassNotFoundException {
        cl = Class.forName(className.toUpperCase());

        Annotation[] annotations = cl.getAnnotations();
        for (Annotation annotation : annotations) {
            System.out.println(annotation);
            if (annotation instanceof Operation) {
                return (Operation) annotation;
            }
        }
        throw new ClassNotFoundException(className);
    }
}
