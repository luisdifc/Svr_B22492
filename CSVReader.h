#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <sstream> 
#include <iostream> 
#include <fstream> 
 
using namespace std;

class CSVReader {

//Aributos
public:
	

//Metodos
public:
    CSVReader();
    virtual ~CSVReader();

    vector<float> parse1DCsvFile(string inputFileName);
    vector<vector<float>> parse2DCsvFile(string inputFileName);
};

#endif // CSVREADER_H
