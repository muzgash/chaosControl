#ifndef CHUA_H
#define CHUA_H

#include<vector>

using namespace std;

class chua{
  public:
    chua(double a, double b,double mm0, double mm1,
	 double sstep, double ttf);
	       
    vector<double>* find();
    
    //mutators
    void setParameters(double *params);
    void setControlWeight(double c);
    void setPeriod(double T);
    void setInitialConditions(double *ic);
    void setTransientTime(double tt);
    void setFinalTime(double ft);
    void setStep(double s);
    
  private:
    //parameters
    double gamma,alpha,beta,m0,m1;
    //parameter of possible control
    double C;
    
    double step,finalTime, transientTime, period;
    double *err;
    double *pos;
    vector<double> *timeSeries;
    
};

int equations(double t, const double y[], double f[],
	   void *params);

#endif