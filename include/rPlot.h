#ifndef RECURRENCEPLOT_H
#define RECURRENCEPLOT_H

#include<iostream>
#include<vector>


using namespace std;

class rplot{
  public:
    rplot();
    
    void plot();
    void build(vector<double> tSeries); //builds the matrix to plot in parallel 
    void build(); //builds the matrix with chua's time series
    //mutators
    
    //accesors
    getMatrix();
    
  private:
    vector<vector<double>> matrix;
    vector<double> chua; //timeSeriesof chua's circuit
};



#endif