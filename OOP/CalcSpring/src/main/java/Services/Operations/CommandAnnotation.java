package Services.Operations;

//задает имя операции
//аннотация с параметрами, принимающая имя операции
public @interface CommandAnnotation {
    String getName();
    void setName();
}
