#ifndef BIF_H
#define BIF_H

#include<vector>

using namespace std;

class bif{
  public:
    bif();
    
    void find();
    
    //accessors
    
    
    
    //mutators
    
    void setGamma(double param);
    
  private:
    double parameters[5];
    double pos[3];
    double err[3];
    double step;
    
};



#endif