#include<rossler.h>
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
  f[2] = b+y[0]*y[2];
  f[3] = -c*y[2]+d*y[3]+C*(F-y[1]);
  return GSL_SUCCESS;
}

rossler::rossler(double aa, double bb,double cc, double dd,
	   double sstep, double ttf){
	    
  isPeriodic=2;
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
  transientTime=200;
  period=2.85;
  sampleRating=finalTime/step;
}

//mutators
void rossler::setParameters(double *params){
  a=params[0];
  b=params[1];
  c=params[2];
  d=params[3];
}

void rossler::setControlWeight(double c){   
  C=c; 
}

void rossler::setPeriod(double T){
  period=T;
}

void rossler::setInitialConditions(double *ic){
  pos[0]=ic[0];
  pos[1]=ic[1];
  pos[2]=ic[2];
  pos[3]=ic[3];
}

void rossler::setTransientTime(double tt){
  transientTime=tt;
}

void rossler::setFinalTime(double ft){
  finalTime=ft;
}

void rossler::setStep(double s){
  step=s;
}

 vector<double>* rossler::get_ts2(){
  return timeSeries2;
 }

//UPO finding
vector<double>* rossler::pyragas(){

  vector<double> feedback;//2.456;
  double params[6]={a,b,c,d,0.0,0.0};//crear atributo de clase

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
    feedback.push_back(pos[3]);
//     cout<<pos[0]<<" "<<pos[1]<<endl;
  }
  params[4]=C; //se agrega el término de control diferente de cero
//   cout<<"t= "<<t<<" period="<<20*period<<" FB size="<<feedback.size()<<endl;
//   cout.flush();
  //now the control comes
  double periodicity0[4][30],periodicity1[4][30]; //variables to know if there's periodicity
  int o=0,p=0;//para avanzar en los arreglos de periodicidad
  for(i=0;i<(int)((finalTime-transientTime-period)/period);i++){
    int n=0;
    while(t<transientTime+(i+1)*period){
      params[5]=feedback[n];
      sys={equations,NULL,4,&params};
      if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
      if(t>transientTime+200*period){
	
	timeSeries->push_back(pos[0]);
	timeSeries2->push_back(pos[1]);
	//relleno los arreglos a comparar para determinar periodicidad
// 	cout<<t<<" "<<transientTime+200*period<<" "<<transientTime+202*period<<endl;
	if(o<30 && t>transientTime+201*period && t<transientTime+203*period){
	  periodicity0[0][o]=pos[0];
	  periodicity0[1][o]=pos[1];
	  periodicity0[2][o]=pos[2];
	  periodicity0[3][o]=pos[3];
	  o++;
	  cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<pos[3]<<endl;
	  cout.flush();
	}
// 	cout<<endl;
	if(p<30 && t<transientTime+205*period && t>transientTime+203*period){
	  periodicity1[0][p]=pos[0];
	  periodicity1[1][p]=pos[1];
	  periodicity1[2][p]=pos[2];
	  periodicity1[3][p]=pos[3];
	  p++;
	  cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<pos[3]<<endl;
	  cout.flush();
	}
// 	cout<<endl;
	if(areEqual(periodicity0,periodicity1)) isPeriodic=0;
	else isPeriodic=1;
      }
//       else cout<<pos[0]<<" "<<pos[1]<<endl;
//       cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
//       cout<<pos[1];
      feedback[n]=pos[3];
//       cout<<n;
//       cout.flush();
      n++;
      t=t+step;
    }
    if(isPeriodic==false) break;
  }
    
  
  gsl_odeiv_step_free(WT);
  
  return timeSeries;
}

bool rossler::attractor(){
  double params[6]={a,b,c,d,0.0,0.0};//crear atributo de clase

  const gsl_odeiv_step_type *T=gsl_odeiv_step_rk4;
  gsl_odeiv_step *WT=gsl_odeiv_step_alloc(T,4);
  gsl_odeiv_system sys={equations,NULL,4,&params};
  
  double t=0.0;

  //transient
  for(t=0.0;t<transientTime;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
  }
  //filling of the feedback vector
  for(t=transientTime;t<finalTime;t+=step){
    if(gsl_odeiv_step_apply(WT,t,step,pos,err,NULL,NULL,&sys)!=GSL_SUCCESS) break;
//     cout<<pos[0]<<" "<<pos[3]<<endl;
  }
  return true;
}

int rossler::areEqual(double a[4][30], double b[4][30]){
  int count=0;
  double tol=0.02;
  for(int i=0;i<30;i++){
    double mod=0;
    for(int j=0;j<4;j++){//square distance
      mod+=pow(a[j][i]-b[j][i],2);
    }
    if(mod<=tol) count++;
  }
  
  if(count>10) {cout<<count;return true;}
  else return false;
}
