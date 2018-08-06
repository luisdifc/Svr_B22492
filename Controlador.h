#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <cstdlib> 
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime> 
#include<fstream>
#include<string>

#include "CSVReader.h"
#include "Svr.h"

using namespace std;

class Controlador {

//Aributos
public:
	

//Metodos
public:
    Controlador();
    virtual ~Controlador();
    int run(int argc, char** argv); //corre el juego

    void vectorModule (vector<float> vector);
};

#endif // CONTROLADOR_H
