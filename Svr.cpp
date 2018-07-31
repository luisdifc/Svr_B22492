/**
*@file Svr.cpp
*@version 1.0
*@date 29/06/18
*@author Luis Diego Fernandez
*@title SVM for Regressions
*@brief Clase que implementa la maquina de soporte vectorial con capaciada de hacer regresiones. Las ecuaciones que se mencionan en los comentarios 
*estan sujetas al metodo matematico observado en http://luthuli.cs.uiuc.edu/~daf/courses/optimization/Papers/smoTR.pdf
*/

#include "Svr.h"

/*! \brief Constructor para crear un Lobo con datos especificos.
 *
 *  \param C Parametro que le da a la maquina control de como maneja errores.
 *  \param tol Tolerancia en cuando al error que puede haber al clasificar un dato.
 *  \param kernelType Tipo de funcion de kernel a utilizar.
 *  \param useLinearOptim Bandera para usar o no una optimizacion lineal en el entrenamiento de la maquina.
 *  \param Xdata Vector de vectores de datos de entrenamiento de entrada.
 *  \param YData Vector de datos obejtivo de entramiento.
 */
Svr::Svr(float C, float tol, int kernelType, int useLinearOptim, vector<vector<float>> XData, vector<float> YData) 
{
    cout << "\nConstruyendo un SVR..!\n" << endl;

    this->X = XData;
    this->Y = YData;

    GiveSizeToMatrix(this->X.size(), this->X[0].size(), this->m);
    GiveSizeToMatrix(this->X.size(), this->X[0].size(), this->n);

	this->FillWithCeros(this->X.size(), this->alphas);
	this->FillWithCeros(this->m.size(), this->errors);
	this->FillWithCeros(this->X[0].size(), this->w);

    this->b = 0;
    this->eps = 0.001;
    this->C = C;
    this->tol = tol;	

    this->kernelType = kernelType;
    this->useLinearOptim = useLinearOptim;
}


/*! \brief Destructor.
 */
Svr::~Svr() 
{

}	


/*! \brief Calcula la salida del SVM para un dato en la posicion i
 *
 *  \param i Posicion del vector X[i] del cual se va a calcular la salida del SVM.
 *  \return Si el valor de retorno es -1 hubo un error.
 */
float Svr::Output(int i) 
{
	// cout << "Output" << endl;
	int error = -1;
	float sum = 0;

	if (this->useLinearOptim == 1) 
	{
		//Ecuacion 1
		return (this->DotProduct(this->w, this->X[i]) - this->b);
	} 
	else 
	{
		for (int index = 0; index < this->m.size(); index++) 
		{
			//Ecuacion 10
			sum += (this->alphas[index] * this->Y[index] * this->Kernel(this->X[index], this->X[i]));
		}
		return sum - this->b;
	}

	return error;
}


/*! \brief Metodo que tratar de solucionar el problema analiticamente trabajando con solo 2 alfas a la vez.
 *
 *  \param i1 Indice del primer dato con el que se va a trabajar.
 *  \param i2 Indice del segundo dato con el que se va a trabajar.
 *  \param a2 Valor del alfa correspondiente a i2.
 *  \param y2 Valor objetivo del inidice correspondiente a i2.
 *  \param E2 Valor del error correspodiente a i2.
 *  \param x2 Vector correspondiente al indice i1 en la matriz de datos de entrenamiento X.
 *  \return 1 si calculo el paso con exito.
 */
int Svr::TakeStep(int i1, int i2, float a2, float y2, float E2, vector<float> x2) 
{
	// cout << "TakeStep" << endl;

	//inicializacion de las variables a utilizar con un valor de 0
	float a1 = 0.0;
	float y1 = 0.0;  
	vector<float> x1;
	float E1 = 0.0;

	float s = 0.0;
	float L = 0.0;
	float H = 0.0;
	float k11 = 0.0;
	float k12 = 0.0;
	float k22 = 0.0;
	float eta = 0.0;
	float a1New = 0.0;
	float a2New = 0.0;
	float f1 = 0.0;
	float f2 = 0.0;
	float L1 = 0.0;
	float H1 = 0.0;
	float Lobj = 0.0;
	float Hobj = 0.0;
	float newB = 0.0;
	float deltaB = 0.0;
	float delta1 = 0.0;
	float delta2 = 0.0;

	if (i1 == i2) 
	{
		return 0;
	}

	a1 = this->alphas[i1];
	y1 = this->Y[i1];
	x1 = this->X[i1];
	E1 = this->GetError(i1);

	s = y1 * y2;

	//Calcular los limites del nuevo valor de a2
	if (y1 != y2) 
	{
		//Ecuacion 13
		L = GetMax(0.0, a2 - a1);
		H = GetMin(this->C, this->C + a2 - a1);
	}
	else 
	{
		//Ecuacion 14
		L = GetMax(0.0, a2 + a1 - this->C);
		H = GetMin(this->C, a2 + a1);
	}

	if (L == H) 
	{
		return 0;
	}

	k11 = this->Kernel(x1, x1);
	k12 = this->Kernel(x1, x2);
	k22 = this->Kernel(x2, x2);

	//Calcular la segunda derivada the la funcion objetivo a lo largo de la diagonal
	//Ecuacion 15
	eta = k11 + k22 - 2 * k12;

	if (eta > 0 ) 
	{
		//Ecuacion 16
		a2New = a2 + y2 * (E1 - E2) / eta;

		//Guardar el valor del nuevo a2
		//Ecuacion 17
		if (a2New < L) 
		{
			a2New = L;
		}
		else 
		{
			if (a2New > H) 
			{
				a2New = H;
			}
		}
	}
	else 
	{
		//Bajo algunas circunstancias extrannas el valor de eta no va a ser positivo y se maneja el caso
		//Ecuacion 19
		f1 = y1 * (E1 + this->b) - a1 * k11 - s * a2 * k12;
		f2 = y2 * (E2 + this->b) - s * a1 * k12 - a2 * k22;
		L1 = a1 + s * (a2 - L);
		H1 = a1 + s * (a2 - H);
		Lobj = L1 * f1 + L * f2 + 0.5 * pow(L1, 2) * k11 + 0.5 * pow(L, 2) * k22 + s * L * L1 * k12;
		Hobj = H1 * f1 + H * f2 + 0.5 * pow(H1, 2) * k11 + 0.5 * pow(H, 2) * k22 + s * H * H1 * k12;
		
		if (Lobj < Hobj - this->eps) 
		{
			a2New = L;
		}
		else 
		{
			if (Lobj > Hobj + this->eps) 
			{
				a2New = H;
			}
			else 
			{
				a2New = a2;
			}
		}
	}

	//If alpha2 did not change enough the algorithm returns without updating the multipliers
	//Si a2 no cambia lo suficiente el algoritmo retorna sin actulizar los multiplicadores
	if (abs(a2New - a2) < (this->eps * (a2New + a2 + this->eps))) 
	{
		return 0;
	}

	//Ecuacion 18
	a1New = a1 + s * (a2 - a2New);
	newB = this->ComputeB(E1, a1, a1New, a2New, k11, k12, k22, y1, y2, a2, E2);
	deltaB = newB - this->b;
	this->b = newB;

	//Ecuacion 22
	if (this->useLinearOptim) 
	{
		this->w = VectorSum(this->w, VectorByScalar(x1, (y1*(a1New - a1))));
		this->w = VectorSum(this->w, VectorByScalar(x2, (y2*(a2New - a2))));
	}

	//Actualizar el error utilizando lo nuevos valores de los multiplicadores de Lagrange
	delta1 = y1 * (a1New - a1);
	delta2 = y2 * (a2New - a2);

	//Actualizar el error
	for (int index = 0; index < this->m.size(); index++) 
	{
		if (0 < this->alphas[index] && this->alphas[index] < this->C) 
		{
			this->errors[index] += delta1 * this->Kernel(x1, this->X[index]) + delta2 * this->Kernel(x2, this->X[index]) - deltaB;
		}
	}

	this->errors[i1] = 0;
	this->errors[i2] = 0;

	this->alphas[i1] = a1New;
	this->alphas[i2] = a2New;

	return 1;
}


/*! \brief Calcula el error para un dato en la posicion i1
 *
 *  \param i1 Posicion en el vector de alfas del cual se va a calcular el error
 *  \return Valor del error.
 */
float Svr::GetError(int i1) 
{
	// cout << "GetError" << endl;
	int error = -1;

	if (0 < this->alphas[i1] && this->alphas[i1] < this->C) 
	{
		return this->errors[i1];
	}
	else 
	{
		return this->Output(i1) - this->Y[i1];
	}

	return (float)error;
}


/*! \brief Truco del kernel. Implementado para un kernel producto punto lineal.
 *
 *  \param v1 Primer vector de entrada.
 *  \param v2 Segundo vector de entrada.
 *  \return resultado del truco del kernel.
 */
float Svr::Kernel(vector<float> v1, vector<float> v2) 
{
	// cout << "Kernel" << endl;
	int error = -1;

	if (this->kernelType == 1) 
	{
		return this->DotProduct(v1, v2); //linear kernel
	}

	return error;
}


/*! \brief Metodo para calcular el valor de b.
 *
 *  \param E1 Error del indice i1.
 *  \param a1 Valor de alfa para el indice i1.
 *  \param a1New Valor calculado nuevo del alfa para el indice i1.
 *  \param a2New Valor calculado nuevo del alfa para el indice i2.
 *  \param k11 Valor de la funcion kernel entre el vector X indice i1 con el mismo.
 *  \param k12 Valor de la funcion kernel entre el vector X indice i1 el vector X indice i2.
 *  \param k22 Valor de la funcion kernel entre el vector X indice i2 con el mismo.
 *  \param y1 Valor objetivo para el indice i1.
 *  \param y2 Valor objetivo para el indice i2.
 *  \param a2 Valor de alfa para el indice i2.
 *  \param E2 Error del indice i2.
 *  \return Valor calculado del b.
 */
float Svr::ComputeB (float E1, float a1, float a1New, float a2New, float k11, float k12, float k22, float y1, float y2, float a2, float E2) 
{
	// cout << "ComputeB" << endl;
	float b1 = 0.0;
	float b2 = 0.0;
	float newB = 0.0;

	//Ecuacion 20
	b1 = E1 + y1 * (a1New - a1) * k11 + y2 * (a2New - a2) * k12 + this->b;

	//Ecuacion 21
	b2 = E2 + y1 * (a1New - a1) * k12 + y2 * (a2New - a2) * k22 + this->b;

	if (0 < a1New && this->C > a1New) 
	{
		newB = b1;
	}
	else 
	{
		if (0 < a2New && this->C > a2New) 
		{
			newB = b2;
		} 
		else 
		{
			newB = (b1 + b2) / 2.0;
		}
	}

	return newB;
}


/*! \brief Metodo para calcular el vector de pesos w y se guarda en el atributo del SVR de la clase.
 *
 *  \param multipliers Vector que contiene los multiplicadores de Lagrange.
 *  \param X Vector de vectores de los datos de entrada.
 *  \param y Vector de valores objetivo.
 */
void Svr::ComputeW(vector<float> multipliers, vector< vector<float> > X, vector<float> y) 
{
	// cout << "ComputeW" << endl;
	vector<float> w;
	vector<float> vecByScal;
	this->FillWithCeros(y.size(), w);

	float sum = 0.0;
	float sum1 = 0.0;
	float sum2 = 0.0;

	for (int index1 = 0; index1 < y.size(); index1++) 
	{
		for (int index2 = 0; index2 <X[index1].size(); index2++) 
		{
			w[index2] += multipliers[index1] * y[index1] * X[index1][index2];
		}
	}

	this->w = w;
	this->w.resize(X[0].size());
}


/*! \brief Calcula el producto de un escalar y un vector.
 *
 *  \param v1 Vector que va a ser multiplicado.
 *  \param scalar Scalar que va a multiplicar el vector. 
 *  \return Result vector. 
 */
vector<float> Svr::VectorByScalar (vector<float> v1, float scalar) 
{
	// cout << "VectorByScalar" << endl;
	vector<float> result;

	for (int index = 0; index < v1.size(); index++) 
	{
		result.push_back(v1[index] * scalar);
	}	

	return result;	
}


/*! \brief Calculo de la primera heuristica del algoritmo SMO.
 *
 *  \return Resultado de la primera heuristica.
 */
float Svr::FirstHeuristic() 
{
	// cout << "FirstHeuristic" << endl;
	int numChanged = 0;
	vector<int> nonBoundIndexes = GetNonBoundIndexes();
	
	for (int index = 0; index < nonBoundIndexes.size(); index++) 
	{
		numChanged += this->ExamineExample(nonBoundIndexes[index]);
	}

	return numChanged;
}


/*! \brief Calculo de la segunda heuristica del algoritmo SMO.
 *
 *  \return Resultado de la segunda heuristica.
 */
float Svr::SecondHeuristic(vector<int> nonBoundIndices, float E2) 
{
	// cout << "SecondHeuristic" << endl;
	float i1 = -1;
	float E1 = 0.0;
	float step = 0.0;
	float max = 0.0;

	if (nonBoundIndices.size() > 1) 
	{
		max = 0;
		for (int index = 0; index < nonBoundIndices.size(); index++) 
		{
			E1 = this->errors[nonBoundIndices[index]] - this->Y[nonBoundIndices[index]];
			step = abs(E1 - E2);
			if (step > max) 
			{
				max = step;
				i1 = nonBoundIndices[index];
			}
		}
	}

	return i1;
}


/*! \brief Calculo de los indices donde el valor de entrada esta fuera del tubo de sensibilidad.
 *
 *  \return Vector con los valores de los alfas que corresponden a datos fuera del tubo de sensibilidad.
 */
vector<int> Svr::GetNonBoundIndexes() 
{
	// cout << "GetNonBoundIndexes" << endl;
	vector<int> result;
	for (int index = 0; index < this->alphas.size(); index++) 
	{
		if (this->alphas[index] > 0 && this->alphas[index] < this->C) 
		{
			result.push_back(index);
		}
		else 
		{
			//continue
		}
	}

	return result;
}


/*! \brief Metodo que revisa los valores de entrada, calculas las heuristicas y revisa contras las condiciones de KKT.
 *
 *  \param i2 Indice del segundo valor del alfa que va a ser utilizado.
 *  \return 0 si las condiciones de KKT se cumplen, uno en otro caso.
 */
int Svr::ExamineExample (int i2) 
{
	// cout << "ExamineExample" << endl;
	float y2 = this->Y[i2];
	float a2 = this->alphas[i2];
	vector<float> x2 = this->X[i2]; 
	float E2 = this->GetError(i2);

	float r2 = 0.0;
	vector<int> nonBoundIndexes;
	int i1 = 0;
	int rand = 0;

	r2 = E2 * y2;

	if (! ( (r2 < (-1*this->tol) && a2 < this->C) || (r2 > this->tol && a2 > 0) ) ) 
	{
		//The KKT conditions are met, SMO looks at another example
		return 0;
	}

	//Second Heuristic A: choose the Lagrange multiplier which maximezes the absolute error
	nonBoundIndexes = GetNonBoundIndexes();
	i1 = SecondHeuristic(nonBoundIndexes, E2);

	if (i1 >= 0 && this->TakeStep(i1, i2, a2, y2, E2, x2) == 1) 
	{
		return 1;
	}

	
	//Second Heuristic B: look for examples making positive progress by looping over all non zero and non C alpha
	//starting at a random point 
	if (nonBoundIndexes.size() > 0) 
	{
		rand = RandNumGenerator(0, nonBoundIndexes.size());
		for (int index = rand; index < nonBoundIndexes.size(); index++) 
		{
			if (this->TakeStep(nonBoundIndexes[index], i2, a2, y2, E2, x2)) 
			{
				return 1;
			}
		}

		for (int index = 0; index < rand; index++) 
		{
			if (this->TakeStep(nonBoundIndexes[index], i2, a2, y2, E2, x2)) 
			{
				return 1;
			}
		}
	}

	rand = RandNumGenerator(0, this->m.size());
	for (int index = rand; index < m.size(); index++) 
	{
		if (this->TakeStep(index, i2, a2, y2, E2, x2)) 
		{
			return 1;
		}
	}

	for (int index = 0; index < rand; index++) 
	{
		if (this->TakeStep(index, i2, a2, y2, E2, x2)) 
		{
			return 1;
		}
	}
	//In extremely degenerate circumstances the SMO skips the first example
	return 0;
}


/*! \brief Rutina que corre la totalidad del algoritmo SMO.
 */
void Svr::MainRoutine() 
{
	// cout << "MainRoutine" << endl;
	float numChanged = 0.0;
	int examineAll = 1;

	while (numChanged > 0 || examineAll) 
	{
		numChanged = 0.0;

		if (examineAll == 1) 
		{
			for (int index = 0; index < this->m.size(); index++) 
			{
				numChanged += this->ExamineExample(index);
			}
		}
		else 
		{
			numChanged += this->FirstHeuristic();
		}

		if (examineAll == 1) 
		{
			examineAll = 0;
		}
		else 
		{
			if (numChanged == 0) 
			{
				examineAll = 1;
			}
		}
	}
}


/*! \brief Metodo que hace la prediccion de modo clasificacion sobre un nuevo cojunto de datos de entrada.
 *
 *  \param newX Nuevos datos a los cuales se les requiere correrle el SVM para realizar una clasificacion.
 *  \return Valor de la clasificacion.
 */
float Svr::Predict(vector<float> newX) 
{
	// cout << "Predict" << endl;
	float newY = 0.0;
	newY = DotProduct(newX, this->w);

	return newY - this->b;
}	


/*! \brief Metodo que hace la prediccion en modo regresion.
 *
 *  \param newX Nuevos datos a los cuales se les requiere correrle el SVR para realizar prediccion del nuevo valor y.
 *  \return Valor de la regresion.
 */
float Svr::PredictRegression(vector<float> newX) 
{	
	float result;

	for (int index = 0; index < this->X.size(); index++) 
	{
		if (this->alphas[index] != 0) {
			result += (this->alphas[index] * this->Kernel(this->X[index], newX));	
		}
	}

	return result - this->b;
}


/*! \brief Metodo que realiza el calculo del producto punto entre 2 vectores del mismo tamanno.
 *
 *  \param v1 Primer vector.
 *  \param v2 Segundo vector.
 *  \return Valor del producto punto.
 */
float Svr::DotProduct (std::vector<float> v1, std::vector<float> v2) 
{
	// cout << "DotProduct" << endl;
	float result = 0;

	for (int index = 0; index < v1.size(); index++) 
	{
		result += v1[index] * v2[index];
	}

	return result;
}


/*! \brief Metodo que realiza la suma de dos vectores.
 *
 *  \param v1 Primer vector.
 *  \param v2 Segundo vector.
 *  \return Vector que contiene el valor del la suma de los vectores.
 */
vector<float> Svr::VectorSum(vector<float> v1, vector<float> v2) 
{
	// cout << "VectorSum" << endl;
	vector<float> result;

	for (int index = 0; index < v1.size(); index++) 
	{
		result.push_back(v1[index] + v2[index]);
	}	

	return result;		
}


/*! \brief Metodo que a llena de ceros un vector.
 *
 *  \param size Tamanno del vector.
 *  \param &vector Referencia del vector que se quiere llenar con ceros.
 *  \return India que se termino de correr con exito
 */
int Svr::FillWithCeros(int size, vector<float> &vector) 
{
	// cout << "FillWithCeros" << endl;
	for (int index = 0; index < size; index++) 
	{
		vector.push_back(0);
	}

	return 0;
}


/*! \brief Metodo que devuelve el mayor entre 2 valores tipo float.
 *
 *  \param n1 Primer valor.
 *  \param n2 Segundo valor.
 *  \return El valor mayor.
 */
float Svr::GetMax(float n1, float n2) 
{
	// cout << "GetMax" << endl;
	int error = -1;

	if (n1 > n2 ) 
	{
		return n1;
	}
	else 
	{
		return n2;
	}

	return error;
}


/*! \brief Metodo que devuelve el menor entre 2 valores tipo float.
 *
 *  \param n1 Primer valor.
 *  \param n2 Segundo valor.
 *  \return El valor menor.
 */
float Svr::GetMin(float n1, float n2) 
{
	// cout << "GetMin" << endl;
	int error = -1;

	if (n1 < n2 ) 
	{
		return n1;
	}
	else 
	{
		return n2;
	}

	return error;
}


/*! \brief Metodo que genera un numero aleatorio basado en una semilla calculada por la hora del momento y que el valor se encuentre entre 2 numeros.
 *
 *  \param n1 Primer valor del intervalo en el que se puede encontrar el numero aleatorio.
 *  \param n2 Segundo valor del intervalo en el que se puede encontrar el numero aleatorio.
 *  \return Numero aleatorio calculado.
 */
int Svr::RandNumGenerator(int n1, int n2) 
{
	// cout << "RandNumGenerator" << endl;
	int randNum = 0;

	srand((unsigned)time(0)); 
    randNum = (rand()%n2)+n1; 

    return randNum;	
}


/*! \brief Metodo que imprime en pantalla un vector de la forma {x1, x2, ..., xn}
 *
 *  \param vector Vector que se quiere imprimir en pantlla.
 */
void Svr::PrintVector(vector<float> vector) 
{
	// cout << "PrintVector" << endl;
	cout << "{";
	for (int index = 0; index < vector.size(); index++) 
	{
		cout << vector[index]; 
		if (index < vector.size() -1) 
		{
			cout << ",";
		}
	}
	cout << "}\n";
}


/*! \brief Metodo que imprime en pantalla un vector de vectores de la forma {{x11, x12, ..., x1n}, {x21, x22, ..., x2n}, ..., {xn1, xn2, ..., xnn}}
 *
 *  \param vector Vector que se quiere imprimir en pantlla.
 */
void Svr::PrintMatrix(vector< vector<float> > array) 
{
	// cout << "PrintMatrix" << endl;
	cout << "size: " << array.size() << "," << array[0].size() << endl;
	for (int index1 = 0; index1 < array.size(); index1++) 
	{
		cout << "{";
		for (int index2 = 0; index2 < array[index1].size(); index2++) 
		{
			if (index2 == array[index1].size() -1) {
				cout << array[index1][index2];
			}
			else 
			{
				cout << array[index1][index2] << ",";
			}
		}
		cout << "}\n";
		}
	cout << "\n";
}


/*! \brief Metodo que le da un nuevo tamano a un vector de vectores.
 *
 *  \param i Numero de columnas.
 *  \param j Numero de filas.
 *  \param &array Referencia del vector de vectores al que se le quiere hacer el cambio de tamanno.
 */
void Svr::GiveSizeToMatrix(int i, int j, vector< vector<float> > &array) 
{
	// cout << "GiveSizeToMatrix" << endl;
	array.resize(i);

	for (int index1 = 0; index1 < i; index1++) 
	{
		array[index1].resize(j);
	}
}