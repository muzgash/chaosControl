#include<chua.h>
#include<fstream>
#include<vector>
#include<iostream>

using namespace std;

int main(int argc, char **argv){
  cout<<"pasa";
  ofstream atractor;
  ofstream bifDiagram;
  atractor.open("atractor.dat");
  bifDiagram.open("bifurc.dat");
  cout<<"pasa";
  chua cc(9.0,14.286,-1.1428571428571428,-0.7142857142857143,
	  0.01,200.0);
  
  vector<double> *y;
  y=new vector<double>;
//   for(double c=0;c<=1;c+=0.001){
  y=cc.find();
//     for(int i=50;i<150;i++){
//       bifDiagram<<c<<" "<<y[i]<<endl;
//     }
//   }
//   
  atractor.close();
  bifDiagram.close();

  return 0;
}