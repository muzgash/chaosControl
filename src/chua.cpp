#include<chua.h>
#include<gsl/gsl_odeiv.h>
#include<gsl/gsl_errno.h>

#include<iostream>
#include<cmath>

using namespace std;

int equations(double t, const double y[], double f[],
	   void *params){
  ///Parameters are alpha, beta,m0,m1,Control, Feedback in that order
  
  double *par=(double*)params;
  double gamma=0.0;
  double a=par[0];//9.0;
  double b=par[1];//14.286;
  double m0=par[2];//-1.1428571428571428;
  double m1=par[3];//-0.7142857142857143;
  double C=par[4];//0.0;
  double F=par[5];//0.0;
  f[0] = a*(y[1]-y[0]-m1*y[0]-0.5*(m0-m1)*(fabs(y[0]+1)-fabs(y[0]-1)))+C*(F-y[0]);
  f[1] = y[0]-y[1]+y[2];
  f[2] = -b*y[1]-gamma*y[2];
  return GSL_SUCCESS;
}

chua::chua(double a, double b,double mm0, double mm1,
	   double sstep, double ttf){
  timeSeries=new vector<double>;
  alpha=a;
  beta=b;
  m0=mm0;
  m1=mm1;
  C=0.0;
  step=sstep;
  finalTime=ttf;
  pos=new double[3];
  pos[0]=-1.0;
  pos[1]=-0.3;
  pos[2]=0.1;
  err=new double[3];
}

//mutators
void chua::setParameters(double *params){
  alpha=params[0];
  beta=params[1];
  m0=params[2];
  m1=params[3];
}

void chua::setControlWeight(double c){   
  C=c; 
}

void chua::setPeriod(double T){
  period=T;
}

void chua::setInitialConditions(double *ic){
  pos[0]=ic[0];
  pos[1]=ic[1];
  pos[2]=ic[2];
}

void chua::setTransientTime(double tt){
  transientTime=tt;
}

void chua::setFinalTime(double ft){
  finalTime=ft;
}

void chua::setStep(double s){
  step=s;
}


//UPO finding
vector<double>* chua::find(){
  cout<<"pasa";
  vector<double> feedback;//2.456;
  double params[6]={alpha,beta,m0,m1,C,0.0};//crear atributo de clase
  cout<<"pasa";
  const gsl_odeiv_step_type *T=gsl_odeiv_step_rk4;
  gsl_odeiv_step *WT=gsl_odeiv_step_alloc(T,3);
  gsl_odeiv_system sys={equations,NULL,3,&params};
  
  int i=0;
  double t=0.0;
  cout<<"pasa";
  //transient
  for(t=0.0;t<transientTime;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
  }
  //filling of the feedback vector
  for(t=transientTime;t=transientTime+period;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
    feedback.push_back(pos[1]);
  }
  
  //now the control comes
  for(i=0;i<(int)((finalTime-transientTime-period)/period);i++){
    int n=0;
    while(t<i*period){
      params[6]=feedback[n];
      sys={equations,NULL,3,&params};
      if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
      if(t>transientTime+2*period){
	timeSeries->push_back(pos[1]);
      }
      feedback[n]=pos[1];
      cout<<n;
      n++;
      t=t+step;
    }}
    
  
  gsl_odeiv_step_free(WT);
  
  return timeSeries;
}
