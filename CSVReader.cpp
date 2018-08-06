/**
*@file CSVReader.cpp
*@version 1.0
*@date 29/06/18
*@author Luis Diego Fernandez
*@title SVM for Regressions
*@brief Clase encargada de hacer lectura de archivos formato *.csv y parsear sus datos para que el programa los utilice.
*/

#include "CSVReader.h"


/*! \brief Constructor por defecto.
 */
CSVReader::CSVReader() 
{

}


/*! \brief Destructor.
 */
CSVReader::~CSVReader() 
{

}
 

/*! \brief Metodo que lee un archivo *.csv y lo pasa a una tabla como un vector de vectores de datos tipo double. El archivo de entrada tiene mas de una columna.
 *
 *  \param inputFileName camino completo al archivo que se quiere leer.
 *  \return result Vector de vectores que contiene los datos contenidos en el archivo de entrada.
 */
vector<vector<double>> CSVReader::parse2DCsvFile(string inputFileName) 
{
    vector<vector<double> > result;
    ifstream inputFile(inputFileName);
    int index = 0;
 
    while (inputFile) 
    {
        index++;
        string s;

        if (!getline(inputFile, s)) break;
        if (s[0] != '#') 
        {
            istringstream ss(s);
            vector<double> record;
 
            while (ss) 
            {
                string line;
                
                if (!getline(ss, line, ','))
                    break;
                try 
                {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) 
                {
                    cout << "Valor NaN encontrado en el archivo " << inputFileName << " linea " << index
                         << endl;
                    e.what();
                }
            }
 
            result.push_back(record);
        }
    }
 
    if (!inputFile.eof()) 
    {
        cerr << "No se pudo leer el archivo." << inputFileName << "\n";
        __throw_invalid_argument("No se encontro el archivo.");
    }
 
    return result;
}
 

/*! \brief Metodo que lee un archivo *.csv y lo pasa a una tabla como un vector de datos tipo double. El archivo de entrada tiene una sola columna.
 *
 *  \param inputFileName camino completo al archivo que se quiere leer.
 *  \return result Vector que contiene los datos contenidos en el archivo de entrada.
 */
vector<double> CSVReader::parse1DCsvFile(string inputFileName) 
{ 
    vector<double> result;
    ifstream inputFile(inputFileName);
    int index = 0;
 
    while (inputFile) 
    {
        index++;
        string s;

        if (!getline(inputFile, s)) break;
        if (s[0] != '#') 
        {
            istringstream ss(s);
            vector<double> record;
 
            while (ss) 
            {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try 
                {
                    result.push_back(stof(line));
                }
                catch (const std::invalid_argument e) 
                {
                    cout << "Valor NaN encontrado en el archivo " << inputFileName << " linea " << index
                         << endl;
                    e.what();
                }
            }
        }
    }
 
    if (!inputFile.eof()) 
    {
        cerr << "No se pudo leer el archivo." << inputFileName << "\n";
        __throw_invalid_argument("No se encontro el archivo.");
    }
 
    return result;
}