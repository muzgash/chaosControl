#ifndef CHUA_H
#define CHUA_H


class chua{
  public:
    chua(double a, double b,double mm0, double mm1,
	 double sstep, double ttf);
	       
    double* find();
    
  private:
    //parameters
    double gamma,alpha,beta,m0,m1;
    //parameter of possible control
    double C;
    
    double step,tf;
    double *err;
    double *pos;
    double *trajectory;
    
};

int equations(double t, const double y[], double f[],
	   void *params);

#endif