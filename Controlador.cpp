/**
*@file Controlador.cpp
*@version 1.0
*@date 29/06/18
*@author Luis Diego Fernandez
*@title SVM for Regressions
*@brief Clase Controlador
*/

#include "Controlador.h"


/*! \brief Constructor por defecto.
 */
Controlador::Controlador() {

}

/*! \brief Destructor.
 */
Controlador::~Controlador() {

}


/*! \brief Metodo que instancia un lector de archivos y una SVR para hacer el entrenamiento y la prediccion.
*/
int Controlador::run(int argc, char** argv) {

	CSVReader reader;

	std::streambuf *psbuf, *backup;
    std::ofstream filestr;

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	//                            CCLE Tests
	//--------------------------------------------------------------------------------
	//-------------------------------------------	-------------------------------------	

	string path = "csvs/";
	path += argv[1];

	// cout << path << endl;

    filestr.open ( path+"/Out1.txt" );
    backup = std::cout.rdbuf();
    psbuf = filestr.rdbuf();
    std::cout.rdbuf(psbuf);

	vector<vector<double>> XData = reader.parse2DCsvFile(path+"/XData.csv");
	vector<vector<double>> XDataR = reader.parse2DCsvFile(path+"/XDataR.csv");
	vector<double> YData = reader.parse1DCsvFile(path+"/YData.csv");

	Svr* maquina1 = new Svr(10, 0.01, 1, 1, XData, YData);
	maquina1->MainRoutine();
	maquina1->ComputeW(maquina1->alphas, maquina1->X, maquina1->Y);
	cout << "w: ";
	maquina1->PrintVector(maquina1->w);
	cout << "b: " << maquina1->b << endl;
	cout << endl;

    std::cout.rdbuf(backup);
    filestr.close();
    delete maquina1;

	cout << "SVR finished running" << endl;

    return 0;
}




	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	//                            OldTests
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------	

/*	// vector<vector<double>> XData = reader.parse2DCsvFile("Data/flowers/XData.csv");
	// vector<double> YData = reader.parse1DCsvFile("Data/flowers/YData.csv");

	vector<vector<double>> XData = reader.parse2DCsvFile("Data/pdfDataset/XDataPdf.csv");
	vector<double> YData = reader.parse1DCsvFile("Data/pdfDataset/YDataPdf.csv");

	// vector<vector<double>> XData = reader.parse2DCsvFile("Data/salary/Data_X.csv");
	// vector<double> YData = reader.parse1DCsvFile("Data/salary/Data_Y.csv");

	// vector<vector<double>> XData = reader.parse2DCsvFile("Data/ccle/XData2.csv");
	// vector<double> YData = reader.parse1DCsvFile("Data/ccle/Y2.csv");

	//                     C  tol  K L.O
	Svr* maquina = new Svr(10, 0.01, 1, 1, XData, YData); //100 

	maquina->MainRoutine();
	maquina->ComputeW(maquina->alphas, maquina->X, maquina->Y);
	cout << "w: ";
	maquina->PrintVector(maquina->w);
	cout << "b: " << maquina->b << endl;
	cout << endl;

    std::cout.rdbuf(backup);        // restore cout's original streambuf

    filestr.close();

	// cout << "New y1(+): " << maquina->Predict({6.2,2.9,4.3,1.3}) << endl;
	// cout << "New y2(+): " << maquina->Predict({6.1,3,4.6,1.4}) << endl;
	// cout << "New y3(+): " << maquina->Predict({6.8,2.8,4.8,1.4}) << endl;
	// cout << "New y4(-): " << maquina->Predict({4.9,3,1.4,0.2}) << endl;
	// cout << "New y5(-): " << maquina->Predict({4.7,3.2,1.3,0.2}) << endl;
	// cout << "New y6(-): " << maquina->Predict({4.3,3,1.1,0.1}) << endl;

	// cout << "New y1(+): " << maquina->Predict({1,11}) << endl;
	// cout << "New y2(+): " << maquina->Predict({2,9}) << endl;
	// cout << "New y3(+): " << maquina->Predict({8,8}) << endl;
	// cout << "New y4(+): " << maquina->Predict({6,10}) << endl;
	// cout << "New y5(-): " << maquina->Predict({11,2}) << endl;
	// cout << "New y5(-): " << maquina->Predict({5,5}) << endl;
	// cout << "New y5(-): " << maquina->Predict({9,3}) << endl;
	// cout << "New y5(-): " << maquina->Predict({4,3}) << endl;

	// cout << "Predicted y: " << maquina->PredictRegression({29,19}) << endl;*/