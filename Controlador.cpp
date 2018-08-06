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


void Controlador::vectorModule (vector<float> vector) 
{
	float module = 0;

	for (int index = 0; index < vector.size(); index++) 
	{
		module += vector[index] * vector[index];
		module = module;
	}

	module = sqrt(module);

	for (int index = 0; index < vector.size(); index++) {
		cout << vector[index] / module << "\n";
	}
	cout << "separador" <<endl;
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

    filestr.open ( path+"/Out_Regressions.txt" );
    backup = std::cout.rdbuf();
    psbuf = filestr.rdbuf();
    std::cout.rdbuf(psbuf);

	vector<vector<float>> XData = reader.parse2DCsvFile(path+"/XData.csv");
	vector<vector<float>> XDataR = reader.parse2DCsvFile(path+"/XDataR.csv");
	vector<float> YData = reader.parse1DCsvFile(path+"/YData.csv");

	// vector<float> X1 = reader.parse1DCsvFile(path+"/X1.csv");
	// vector<float> X2 = reader.parse1DCsvFile(path+"/X2.csv");
	// vectorModule(X1);
	// vectorModule(X2);
	// vectorModule(YData);

	Svr* maquina1 = new Svr(10, 0.01, 1, 1, XData, YData);
	maquina1->MainRoutine();
	maquina1->ComputeW(maquina1->alphas, maquina1->X, maquina1->Y);
	cout << "w: ";
	maquina1->PrintVector(maquina1->w);
	cout << "b: " << maquina1->b << endl;
	cout << endl;

	cout << "Alphas: " << endl;
	maquina1->PrintVector(maquina1->alphas);

	cout << "\nRegressions:" << endl;
	maquina1->Vector2Csv(maquina1->PredictRegression(XDataR));
	

    std::cout.rdbuf(backup);
    filestr.close();
    //delete maquina1;

	cout << "SVR finished running" << endl;

    return 0;
}





	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	//                            OldTests
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------	

/*	// vector<vector<float>> XData = reader.parse2DCsvFile("Data/flowers/XData.csv");
	// vector<float> YData = reader.parse1DCsvFile("Data/flowers/YData.csv");

	vector<vector<float>> XData = reader.parse2DCsvFile("Data/pdfDataset/XDataPdf.csv");
	vector<float> YData = reader.parse1DCsvFile("Data/pdfDataset/YDataPdf.csv");

	// vector<vector<float>> XData = reader.parse2DCsvFile("Data/salary/Data_X.csv");
	// vector<float> YData = reader.parse1DCsvFile("Data/salary/Data_Y.csv");

	// vector<vector<float>> XData = reader.parse2DCsvFile("Data/ccle/XData2.csv");
	// vector<float> YData = reader.parse1DCsvFile("Data/ccle/Y2.csv");

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