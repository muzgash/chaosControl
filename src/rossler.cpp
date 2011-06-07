#include<chua.h>
#include<gsl/gsl_odeiv.h>
#include<gsl/gsl_errno.h>

#include<iostream>
#include<cmath>

using namespace std;

int equations(double t, const double y[], double f[],
	   void *params){
  ///Parameters are a, b,c,d,Control, Feedback in that order
  
  double *par=(double*)params;
  double a=par[0];//9.0;
  double b=par[1];//14.286;
  double c=par[2];//-1.1428571428571428;
  double d=par[3];//-0.7142857142857143;
  double C=par[4];//0.0;
  double F=par[5];//0.0;
  f[0] = -y[1]-y[2];
  f[1] = y[0]+a*y[1]+y[3];
  f[2] = b+y[0]*y[2]+C*(F-y[2]);
  f[3] = -c*y[2]+d*y[3];
  return GSL_SUCCESS;
}

chua::chua(double aa, double bb,double cc, double dd,
	   double sstep, double ttf){
  timeSeries=new vector<double>;
  timeSeries2=new vector<double>;
  a=aa;
  b=bb;
  c=cc;
  d=dd;
  C=0.0;
  step=sstep;
  finalTime=ttf;
  pos=new double[4];
  pos[0]=-10.0;//-1.0;
  pos[1]=-6.0;//-0.3;
  pos[2]=0.0;//0.1;
  pos[3]=10.0;
  err=new double[4];
  transientTime=100;
  period=2.85;
  sampleRating=finalTime/step;
}

//mutators
void chua::setParameters(double *params){
  a=params[0];
  b=params[1];
  c=params[2];
  d=params[3];
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
  pos[3]=ic[3];
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

 vector<double>* chua::get_ts2(){
  return timeSeries2;
 }

//UPO finding
vector<double>* chua::pyragas(){

  vector<double> feedback;//2.456;
  double params[6]={a,b,c,d,C,0.0};//crear atributo de clase

  const gsl_odeiv_step_type *T=gsl_odeiv_step_rk4;
  gsl_odeiv_step *WT=gsl_odeiv_step_alloc(T,4);
  gsl_odeiv_system sys={equations,NULL,4,&params};
  
  int i=0;
  double t=0.0;

  //transient
  for(t=0.0;t<transientTime;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
  }
  //filling of the feedback vector
  for(t=transientTime;t<transientTime+period;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
    feedback.push_back(pos[0]);
  }
//   cout<<"t= "<<t<<" period="<<20*period<<" FB size="<<feedback.size()<<endl;
//   cout.flush();
  //now the control comes
  for(i=0;i<(int)((finalTime-transientTime-period)/period);i++){
    int n=0;
    while(t<transientTime+(i+1)*period){
      params[5]=feedback[n];
      sys={equations,NULL,4,&params};
      if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
      if(t>transientTime+100*period){
	timeSeries->push_back(pos[2]);
	timeSeries2->push_back(pos[1]);
// 	cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
// 	cout.flush();
      }
      cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
//       cout<<pos[1];
      feedback[n]=pos[2];
//       cout<<n;
//       cout.flush();
      n++;
      t=t+step;
    }
  }
    
  
  gsl_odeiv_step_free(WT);
  
  return timeSeries;
}

int chua::isPeriodic(){
  double a=0.0,b=0.0;
  for(int i=100;i<500;i++){
    a+=timeSeries->at(i);
    b+=timeSeries->at(i+int(period/step));
  }
  cout<<a/400<<" "<<b/400<<endl;
}
