package ru.nsu.fit.tropin.Operations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

//задает имя операции
//аннотация с параметрами, принимающая имя операции
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface CommandAnnotation {
    String name() default "DEFINE";
}
