/***
 * author: Liz Huancapaza H.
 **/

#ifndef DENSEMATRIX_H
#define	DENSEMATRIX_H

//#include "Types.h"

#include <cassert>
#include <iostream>
#include <vector>

//using namespace std;

//template <class Type>
class DenseMatrix{
private:
    double * _data;
    int _rows;
    int _cols;
    
public:
    /**
     * @brief Constructor.
     * @param rows
     * @param cols
     */
    DenseMatrix(int rows, int cols){
        _rows = rows;
        _cols = cols;
//        _data = new Type[_rows*_cols]();
        _data = new double[_rows*_cols]();
    }
    
    /**
     * @brief Destructor.
     */
    ~DenseMatrix(){
        delete []_data;
    }
    
    /**
     * @brief Update value
     * @param i, row
     * @param j, col
     * @return 
     */
    inline double& at(int i, int j){
        return _data[i*_cols+j];
    }

    int rows(){
        return _rows;
    }    

    int cols(){
        return _cols;
    }    
    /**
     * @brief Print all values.
     */
    void print(){
        int i, j;
        for(i=0; i<_rows; ++i){
            for(j=0; j<_cols; ++j){
                std::cout<<at(i,j)<<"\t";
            }
            std::cout<<std::endl;
        }
    }
    
    /**
     * @brief Print diagonal.
     */
    void printD(){
        int i, j;
        for(i=0; i<_rows; ++i){
            if(i<_cols)
                std::cout<<at(i,i)<<"\t";
        }    
        std::cout<<std::endl;        
    }
    
    void normalized(){
        int row, col;
        double maxv = -1.0;
        for(row=0; row<_rows; ++row){
            for(col=0; col<_cols; ++col){
                if(this->at(row, col)>maxv){
                    maxv = this->at(row, col);
                }
            }
        }
        for(row=0; row<_rows; ++row){
            for(col=0; col<_cols; ++col){
                this->at(row, col) /= maxv;
            }
        }
    }

};
#endif