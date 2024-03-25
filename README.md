# Trabajo Práctico: Device Drivers
***
El objetivo del proyecto es comprender el funcionamiento de los módulos del kernel. A partir de material teórico brindado por los docentes se implementaron 
tres módulos cada uno con una función determinada. El formato que se decidió ordenar los archivos es tal que cada carpeta representa un módulo y en ellas se encontrará
el código C correspondiente y su archivo makefile para facilitar la compilación.
Sin embargo, para mayor información se recomienda la lectura del informe en formato pdf que se encuentra en este repositorio.
***
## Información general
### Tecnologías y material adicional
A continuación se mencionará material adicional que se ha utilizado para la realización de este trabajo:
* [Virtual Box](https://drive.google.com/file/d/1okMXF12jF2ePF_8rbaLlEZFmlI-efc66/view?usp=drive_link)
* [The Linux Kernel Module Programming Guide](https://drive.google.com/file/d/13DDYExiDclWYDp4rIYGxsOYr4hiIS_rL/view?usp=sharing)
* [Enunciado](https://drive.google.com/file/d/1lO1Ny8hYwJaefI_TGdp-iXq5uT3uA_be/view?usp=sharing)
* [Maquina Virtual brindada](https://drive.google.com/file/d/1NrkzMOM3EC9-55VPpU-ZjR1VfAiyE5aF/view?usp=drive_link)

### Implementación del último punto
Se decide explicar en esta área el último punto solicitado en el trabajo práctico debido a que es el más complejo y completo de los solicitados, capaz de brindar una
mayor información sobre los comandos para ejecutar los puntos anteriores sin problemas.
Dicho punto consiste en implementar un módulo capaz de recibir una palabra al escribir en el y devuelva el mensaje al reves.

En primer lugar, para poder realizar la correcta carga y uso del charDeviceRev se debe ubicar en la carpeta moduloCharRev y ejecutar el comando *make*.
```
>> cd moduloCharRev
>> make
```
Luego se realizar la carga del módulo y la verificación de la carga:
```
>> sudo insmod ./charDeviceRev.ko
>> sudo lsmod
```
Posteriormente, se realiza el la creaciónde un carpeta especial para nuestro device, brindandole los correctos permisos para las pruebas de escritura y lectura.
```
>> sudo mknod /dev/charDeviceRev c 240 0
>> sudo chmod 777 /dev/charDeviceRev
```
Realizamos las pruebas de escritura y lectura:
```
>> echo "hola mundo" >> /dev/charDeviceRev
>> cat /dev/charDeviceRev
>> odnum aloh
```
Al terminar de verificar que nuestro módulo realiza lo pedido, no hay que olvidarse de retirarlo de nuestro kernel y limpiar nuestro directorio con los siguientes comandos:
```
>> sudo rmmod ./charDeviceRev.ko
>> sudo rm /dev/charDeviceRev
>> make clean
```
