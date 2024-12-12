# Обнаружение копий себя в локальной сети
Разработать приложение, обнаруживающее копии себя в локальной сети с помощью обмена multicast UDP сообщениями. Приложение должно отслеживать моменты появления и исчезновения других копий себя в локальной сети и при изменениях выводить список IP адресов "живых" копий.

Адрес multicast-группы необходимо передавать параметром в приложение. Приложение должно поддерживать работу как в IPv4 так и в IPv6 сетях, выбирая протокол автоматически в зависимости от переданного адреса группы.

## Usage

### Сreating container image

```shell
docker build -t ntdscvr:v1 .
Sending build context to Docker daemon  23.04kB
.
.
.
Successfully built e887871c0399
Successfully tagged ntdscvr:v1
```
### Running container image 

```shell
 docker run -it --name ntdscvr1 ntdscvr:v1
```
```shell
 docker run -it --name ntdscvr2 ntdscvr:v1
```
### Show running containers

```bash
docker ps -a
CONTAINER ID   IMAGE        COMMAND           CREATED          STATUS                     PORTS      NAMES
e489f6fa9203   ntdscvr:v1   "./netDiscover"   10 seconds ago   Up 9 seconds               8000/tcp   ntdscvr2
17bd8b331acd   ntdscvr:v1   "./netDiscover"   10 seconds ago   Up 15 seconds              8000/tcp   ntdscvr1
```

### Remove container

```bash
docker rm e489f6fa9203
docker rm 17bd8b331acd
```