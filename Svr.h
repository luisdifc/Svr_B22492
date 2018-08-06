#ifndef SVR_H
#define SVR_H

#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include<fstream>
#include<string>

using namespace std;

class Svr {

//Atributos
public:
vector< vector<double> > X;
vector<double> Y;
vector< vector<double> > m;
vector< vector<double> > n;
vector<double> alphas;
vector<double> errors;
vector<double> w;

double b;
double eps; //epsilon
double C;
double tol;

int kernelType; // 1 = linear
int useLinearOptim;
	

//Metodos
public:
    Svr(double C, double tol, int kernelType, int useLinearOptim, vector<vector<double>> XData, vector<double> YData);
    virtual ~Svr();

    //SVR methods
    double Output(int i);
    int TakeStep(int i1, int i2, double a2, double y2, double E2, vector<double> x2);
    double GetError(int i1);
    double Kernel(vector<double> v1, vector<double> v2);
    double ComputeB (double E1, double a1, double a1New, double a2New, double k11, double k12, double k22, double y1, double y2, double a2, double E2);
    void ComputeW(vector<double> multipliers, vector< vector<double> > X, vector<double> y);
    double FirstHeuristic();
    double SecondHeuristic(vector<int> nonBoundIndices, double E2);
    int ExamineExample (int i2);
    double Error(int i2);
    vector<int> GetNonBoundIndexes();
    void MainRoutine();
    double Predict(vector<double> newX);
    double PredictRegression(vector<double> newX);

    //Auxiliary methods
    double DotProduct(vector<double> v1, vector<double> v2);
    vector<double> VectorByScalar(vector<double> v1, double scalar);
    vector<double> VectorSum(vector<double> v1, vector<double> v2);
    int FillWithCeros(int size, vector<double> &vector);
    double GetMax(double n1, double n2);
    double GetMin(double n1, double n2);  
    int RandNumGenerator(int n1, int n2);
    void GiveSizeToMatrix(int i, int j, vector< vector<double> > &array);
    void PrintVector(vector<double> vector);
    void PrintMatrix(vector< vector<double> > array);
    float myRound(float var);
};

#endif // SVR_H