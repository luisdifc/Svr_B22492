Para compilar el codigo
	$ make build

Para correr el codigo
	$ make run

El archivo Controlador.cpp se encarga de coordinar la forma en que se corren las pruebas.
Basicamente es un switch case que corre los distintos casos (los que se ven en la imagen testCases.jpg).
El switch se controla por un ciclo for... Ahora en este momento como prueba puse a correr los casos 1, 14, 15 y 28. 

Al correr con make run los resultados se guardan en la respectiva carpeta donde estan los datos. Eso es
en la carpeta:
	 csvs/<numeroTestCase>  //osea csvs/1, csvs/14, csvs/15, csvs/28

Como solo estaba probando la maquina para estos datos lo que genero nada mas es el plano, 
osea vector de pesos 'w' y valor de 'b'. Que me dan inf's o nan's para estos sets de datos...

El caso default del switch es el caso de prueba que venia con la implementacion del algoritmo, 
ese funciona por si lo quiere correr, es muy basico.

La implementacion es de acuerdo al pdf adjunto, el que habiamos encontrado.
