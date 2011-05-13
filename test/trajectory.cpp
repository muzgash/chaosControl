#include<chua.h>

#include<fstream>
#include<iostream>

// #include<gsl/gsl_odeiv.h>
// #include<gsl/gsl_errno.h>

using namespace std;

#define p(x) cout<<x<<endl

int main(int argc, char **argv){
  
  chua cc(9.0,14.286,-1.1428571428571428,-0.7142857142857143,
	  0.01,200.0);
  
  double *y;
  
  
  y=cc.find();

  return 0;
}