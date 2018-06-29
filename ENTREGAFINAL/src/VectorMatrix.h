/***
 * author: Liz Huancapaza H.
 **/


#ifndef VECTORMATRIX_H
#define	VECTORMATRIX_H

#include "DataSet.h"
#include "Descriptor.h"

#include <cmath>
#include <limits>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>



//namespace App{
//using namespace App;

char* string_as_array(std::string str){
    char *y = new char[str.length() + 1]; // or
    std::strcpy(y, str.c_str());
    return y;
}

//circular reference
//class Descriptor;
//class d;

class Vector{
private:
    int _cols;
    double *_values;

public:
    Vector(int cols){
        _cols = cols;
        _values = new double[cols]();
    }

    /**
     * @brief Destructor.
     */
    ~Vector(){
        delete []_values;
    }

    inline double& at(int i){
        return this->_values[i];
    }
    
    inline int size(){
        return this->_cols;
    }

    inline void print(){
        int i=0;
        for(i=0; i<_cols; ++i)
            std::cout<<_values[i]<<std::endl;
//        std::cout<<std::endl;
    }
};

class VectorMatrix{
private:
    int _rows;
    int _cols;
    int *_classe;
    Vector** _vectors;
public:
    /**
     * @brief Constructor.
     * @param rows, number of rows.
     * @param columns, number of columns.
     */
    VectorMatrix(int rows, int cols){
        _rows = rows;
        _cols = cols;

        _vectors = new Vector*[rows];
        _classe = new int[rows];
        int i;
        for(i=0; i<rows; ++i){
            _vectors[i] = new Vector(cols);
            _classe[i]  = 0;
        }
    }
    int &classe(int i){
        return _classe[i];
    }
    
    ~VectorMatrix(){

        delete []_classe;
        int i;
        for(i=0; i<_rows; ++i){
            delete _vectors[i];
        }
        delete _vectors;
    }

    inline Vector*& at(int i){
        return _vectors[i];
    }
    
    inline double& at(int i, int j){
        return _vectors[i]->at(j);
    }

    int rows(){
        return _rows;
    }
    
    int cols(){
        return _cols;
    }
    
    void print(){
        int i, j;
        for(i=0; i<_rows; ++i){
            _vectors[i]->print();
        }
    }

    static double euclidean(Vector *vi, Vector *vj){
        int i=0;
        int n=vi->size();
        double a = 0.0;
        double d = 0.0;
        for (i=0;i<n; ++i){
            a = vi->at(i) - vj->at(i);
            d += a*a;
        }
        d = std::sqrt(d);
        return d;
    }

    /**
     * @brief Write file vector matrix with meta data
     * @param filename, write filename *.data
     */
    void write(
        std::string filename,
        DataSet *dataset,
        const char *nome,
        const char *shortNome,
        const char *id
    ){
        //seam format that pex-projection
        std::ofstream file(filename.c_str());

//        Corel 1000
//        corel_1000
//        HC
//        HC
//        1HC
//        1000
//        192
//        10
//        Africans;Beach;Monuments;Buses;Dinosaurs;Elephant;Flower;Horse;Mountain;Foot
//        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//        auto *ccc = des->name();
//        std::cout<<des->id()<<std::endl;
        
        int i,j;
        file<<dataset->nameDataSet()<<"\n";
        file<<dataset->nameDataSetShort()<<"\n";
        file<<nome<<"\n";
        file<<shortNome<<"\n";
        file<<id<<"\n";
        file<<_rows<<"\n";
        file<<_cols<<"\n";
        file<<dataset->classes()<<"\n";
        file<<dataset->classesLabels()<<"\n";
        
        for(i=0; i<_cols-1; ++i){
            file<<i+1<<";";
        }
        file<<_cols<<"\n";

        for(i=0; i<_rows-1; ++i){                    
            file<<i;
            for(j=0; j<_cols; ++j)
                file<<";"<<this->at(i, j);
            file<<";"<<this->_classe[i]<<"\n";
        }

        i = _rows-1;
        file<<i;
        for(j=0; j<_cols; ++j)
            file<<";"<<this->at(i, j);

        file<<";"<<this->_classe[i];
        file.close();
    }

    
    /**
     * @brief Normalization by column: std deviation.
     */
    void normalizationColumnsZScore(int coli, int colj){
        double a;
        double avrg = 0.0;
        double stdv = 0.0;
        int i, j;
        for(j=coli; j<colj; ++j){
            for(i=0; i<_rows; ++i)
                avrg += _vectors[i]->at(j);
            
            avrg = avrg/_rows;
            
            for(i=0; i<_rows; ++i){
                a = _vectors[i]->at(j) - avrg;
                stdv += a*a;
            }
            stdv = std::sqrt( stdv/(_rows-1) );
            
            for(i=0; i<_rows; ++i)
                _vectors[i]->at(j) = (_vectors[i]->at(j) - avrg)/(stdv+0.01);
        }
        
    
    }

    /*
     * @brief Normalization all data matrix by column (Min-Max) [0.0, 1.0].
     */
    void normalizationColumnsMinMax(){
        double max;
        double min;
        
        int i, j;
        double val;
        for(j=0; j<_cols; ++j){
            max = std::numeric_limits<double>::min();
            min = std::numeric_limits<double>::max();
            for(i=0; i<_rows; ++i){
                Vector*& v = at(i);
                val = v->at(j);
                if (val>max)
                    max = val;
                if (val<min)
                    min = val;
            }
            for(i=0; i<_rows; ++i){
                Vector*& v = at(i);
                val = v->at(j);
                if(max > min)    
                    v->at(j) = (((val - min))/((max-min)));
                else
                    v->at(j) = 0.0;
            }
        }
    }

};

//}
#endif