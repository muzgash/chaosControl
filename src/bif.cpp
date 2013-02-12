#include<bif.h>
#include<gsl/gsl_odeiv.h>
#include<gsl/gsl_errno.h>

#include<iostream>
#include<cmath>

using namespace std;

int equations(double t, const double y[], double f[],
	   void *params){
  ///Parameters are alpha, beta,m0,m1,gamma, in that order
  
  double *par=(double*)params;  
  double a=par[0];//9.0;
  double b=par[1];//14.286;
  double m0=par[2];//-1.1428571428571428;
  double m1=par[3];//-0.7142857142857143;
  double g=par[4];//0.0
//   cout<<a<<" "<<b<<" "<<g<<" "<<m0<<" "<<m1<<endl;
  f[0] = a*(y[1]-y[0]-m1*y[0]-0.5*(m0-m1)*(fabs(y[0]+1)-fabs(y[0]-1)));
  f[1] = y[0]-y[1]+y[2];
  f[2] = -b*y[1]-g*y[2];
  return GSL_SUCCESS;
}

bif::bif(){
  pos[0]=0.50;//-1.0;
  pos[1]=-0.43;//-0.3;
  pos[2]=0.60;//0.1;
//   parameters=new double[4];
  parameters[0]=9.0;
  parameters[1]=14.286;
  parameters[2]=-1.1428571428571428;
  parameters[3]=-0.7142857142857143;
  parameters[4]=0.0;
  step=0.05;
}

void bif::setGamma(double g){
  parameters[4]=g;
}

void bif::find(){
  double g=-4.0;
  double t=0.0;
  
  const gsl_odeiv_step_type *T=gsl_odeiv_step_rk4;
  gsl_odeiv_step *WT=gsl_odeiv_step_alloc(T,3);
  while(g<4){
    pos[0]=0.50;//-1.0;
    pos[1]=-0.43;//-0.3;
    pos[2]=0.60;//0.1;
    parameters[4]=g;
    gsl_odeiv_system sys={equations,NULL,3,&parameters};
    for(t=0.0;t<400;t+=step){
      if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
      if(t>200){
	if(fabs(pos[1])>2.5) break;
        cout<<g<<" "<<pos[0]<<endl;
      }
      cout.flush();
    }
    g=g+0.001;
  }
}