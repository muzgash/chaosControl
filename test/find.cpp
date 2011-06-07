#include<chua.h>
#include<fstream>
#include<vector>
#include<iostream>
#include<sstream>

using namespace std;

int main(int argc, char **argv){
  ofstream tSData;
  ofstream atractor;
  double p[6]={0.5,1.3,2.25,2.3,2.85,2.9};
  for(int i=0;i<6;i++){
    chua cc(0.25,3.0,0.5,0.05,
	  0.001,400.0);
  
    vector<double> *y,*y2;
    y=new vector<double>;
    y2=new vector<double>;
//   for(double c=0.01;c<=1;c+=0.001){
    cc.setControlWeight(0.4);
  
    cc.setPeriod(p[i]);
    y=cc.pyragas();
    y2=cc.get_ts2();
    
    stringstream name;
    stringstream name2d;
//     name=new stringstream(stringstream::in | stringstream::out);
    name<<"../etc/timeSeries_"<<p[i]<<".dat";
    tSData.open(name.str());
    name2d<<"../etc/timeSeries_"<<p[i]<<"_2d.dat";
    atractor.open(name2d.str());
    
    for(int j=0;j<y->size();j++){
      atractor<<y->at(j)<<" "<<y2->at(j)<<endl;
      tSData<<y->at(j)<<endl;
    }
    tSData.close();
    atractor.close();
  }


//   if(argv[1]=="--plot"){
//     cout<<"Plotting"<<endl;
//     system("python ../etc/plot.py");
//   }
//   return 0;
}