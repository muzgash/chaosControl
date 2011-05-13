#include<chua.h>
#include<cmath>
#include<gsl/gsl_odeiv.h>
#include<gsl/gsl_errno.h>

#include<iostream>
using namespace std;
#define p2(x,y) cout<<x<<" "<<y<<endl
#define p1(x) cout<<x<<endl
#define pp() cout<<"paso"<<endl

int equations(double t, const double y[], double f[],
	   void *params){
  ///Parameters are alpha, beta,m0,m1,Control, Feedback in that order
  
  double *par=(double*)params;
  double gamma=0.0;
  f[0] = par[0]*(y[1]-y[0]-par[3]*y[0]-0.5*(par[2]-par[3])*(fabs(y[0]+1)-fabs(y[0]-1)))+par[4]*(par[5]-y[0]);
  f[1] = y[0]-y[1]+y[2];
  f[2] = -par[1]*y[1]-gamma*y[2];
  return GSL_SUCCESS;
}

chua::chua(double a, double b,double mm0, double mm1,
	   double sstep, double ttf){
  trajectory=new double[100000];
  alpha=a;
  beta=b;
  m0=mm0;
  m1=mm1;
  C=0.0;
  step=sstep;
  tf=ttf;
  double ppos[3]={-1.0,-0.3,0.1};
  pos=new double[3];
  pos=ppos;
  err=new double[3];
}

double* chua::find(){
  double F=0.0;
  double params[6]={alpha,beta,m0,m1,C,F};
  
  const gsl_odeiv_step_type *T=gsl_odeiv_step_rk8pd;
  gsl_odeiv_step *WT=gsl_odeiv_step_alloc(T,3);
  gsl_odeiv_system sys={equations,NULL,3,&params};
  
  int i=0;
  for(double t=0.0;t<tf;t+=step){ 
    if (gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) 
      break;
    cout<<t<<endl;
    if(t>50.0){//transient
      
      trajectory[i]=pos[0];
      i++;
    }
  }
  
  gsl_odeiv_step_free(WT);
  
  return trajectory;
}
