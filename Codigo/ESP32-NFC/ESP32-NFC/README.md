**Cambios necesarios para que funcione**

Es importante cambiar los datos en el *sdfconfig* apartado Example Configuration ya que aquí es donde se tiene que introducir los datos de la red wifi a utilizar (SSID y Contraseña). De esta forma el comedero se podrá conectar a la red y poder visualizar la página web en un navegador de internet.

![image](https://github.com/Rubbit04/ESP-IDF-PetFeeder/assets/73599929/1f3a563e-c6c4-4042-ae4f-75f3183986bc)


Para saber la ip a la cual nos tenemos que conectar con el navegador es necesario compilar y ejecutar el programa, nada más iniciar se indicará si se ha conectado correctamente a la red o no. Si se ha conectado, lo siguiete se saldrá por el terminal es la dirección IP que el router ha asignado al ESP32 y por tanto será esa IP la cual se tiene que introducir en el navegador.

![image](https://github.com/Rubbit04/ESP-IDF-PetFeeder/assets/73599929/8aa65a20-f3aa-4a4f-8186-85ebcce8f524)
