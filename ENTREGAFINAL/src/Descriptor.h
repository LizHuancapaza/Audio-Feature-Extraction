/***
 * author: Liz Huancapaza H.
 **/

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "DenseMatrix.h"
#include "VectorMatrix.h"
#include "ImageIO.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <functional>
#include <vector>
#include <list>
#include <cassert>
//#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <set>

auto reduceColor64(ImageIO *image)->unsigned char *{
    int b = 2;//b=2; n=2**b; n=4; n**3=64;
    int d = std::pow(2,b);//b=2; n=2**b; n=4; n**3=64;
    int e = d*d*d;//b=2; n=2**b; n=4; n**3=64;
    int dv = 256/std::pow(2,b);//b=2; n=2**b; n=4; n**3=64;
    int r = 8 - b;
    
    unsigned char maskBit = 0xFF;
    maskBit = maskBit << (r);
    
    int i, j;
    unsigned char *data = new unsigned char[image->size()];
    maskBit = 64*3;
    for (i = 0; i < image->size(); ++i){
        unsigned char C1 = (image->at(i,0) & maskBit);
        unsigned char C2 = (image->at(i,1) & maskBit);
        unsigned char C3 = (image->at(i,2) & maskBit);

//        unsigned char C1 = (image->at(i,0) & rv)>>2;
//        unsigned char C2 = (image->at(i,1) & rv)>>4;
//        unsigned char C3 = (image->at(i,2) & rv)>>6;
        data[i] = (C1>>2) | (C2>>4) | (C3>>6);

//        int x = C1/(dv);
//        int y = C2/(dv);
//        int z = C3/(dv);
//        data[i] = x + y*d + z*d*d;
        
        
        int c1 = C1;
        int c2 = C2;
        int c3 = C3;
//        int c1 = (C1/64.0)*255;
//        int c2 = (C2/64.0)*255;
//        int c3 = (C3/64.0)*255;
        
        image->at(i,0) = c1;
        image->at(i,1) = c2;
        image->at(i,2) = c3;

//        int num = data[i];
//        if (num>=64-1)
//            std::cout<<num<<" = "<<(int)C1<<":"<<(int)C2<<":"<<(int)C3<<" ;;;"<<e<<std::endl;
            
//        if (c1>=192 || c2>=192 || c3>=192)
//            std::cout<<num<<" = "<<(int)C1<<":"<<(int)C2<<":"<<(int)C3<<std::endl;

    }
//    image->display();
    return data;
}

class Descriptor{    
protected:

    int  _size_features;
    const char * _name_desc;
    const char *_name_desc_short;
    const char *_id_desc;
    
public:
    int size(){
        return _size_features;
    }
    
    const char * name(){
        return _name_desc;
    }
    
    const char * shortName(){
        return _name_desc_short;
    }

    const char * id(){
        return _id_desc;
    }
};

/**
 * Descriptor GLCoocMatrix
 */
class CoocMatrix: public Descriptor{
protected:
    std::vector<int> _dx;
    std::vector<int> _dy;
    
public:
    
    CoocMatrix(
        std::vector<int>dx,
        std::vector<int>dy
    ){        
        _dx = dx;
        _dy = dy;
           
        _size_features = _dx.size()*6;
        _name_desc = "GLCooMatrix";
        _name_desc_short = "GLCM";
        _id_desc = "1";
    }
    
    void compute(
        ImageIO *img,
        Vector *fe
    ){
        int i, j, k, x, y, vi, vj, i_j, xj, yj, gi, gj, ai, aj;
        double pij;
        img->toGrayLuminance();
        int _size = 256;
        int _n = _dx.size();
        DenseMatrix **_mat =  new DenseMatrix*[_n];
        for (i=0; i<_n; ++i){
            _mat[i] = new DenseMatrix(_size, _size);
        }
        
        double *_sum = new double[_n]();
        double _v;
        
        int w = img->width();
        int h = img->height();
        int s = img->size();
    

        //computar as coocorrencias das n matrices
        for (x=0; x<w; ++x){
            for (y=0; y<h; ++y){
                for (i=0; i<_n; ++i){
                    xj = _dx[i]+x;
                    yj = _dy[i]+y;
                    if (xj>=0 && xj<w && yj>=0 && yj<h){
                        gi = img->at(x, y, 0);
                        gj = img->at(xj, yj, 0);
//                        if(gi!=gj){
//                        _v = gi-gj;
//                        _v = std::sqrt(_v*_v);
                        
//                        _mat[i]->at(gi,gj) +=_v;
                            _mat[i]->at(gi,gj)++;
                            _sum[i]++;
//                        _sum[i] += (double)_v;
//                        }

                    }
                }
            }
        }

        //declarar medidas estadisticas de Haralick
        double *_ene = new double[_n]();
        double *_ent = new double[_n]();
        double *_con  = new double[_n]();
        double *_corr = new double[_n]();
        double *_hom = new double[_n]();
        double *_pobmax = new double[_n]();

        double *mr = new double[_n]();
        double *mc = new double[_n]();
        double *o_r = new double[_n]();
        double *o_c = new double[_n]();
        
        DenseMatrix *sumr = new DenseMatrix(_n, _size);
        DenseMatrix *sumc = new DenseMatrix(_n, _size);
        
        /*normalization*/  
        for (i=0; i<_size; ++i){
            for (j=0; j<_size; ++j){
                for (k=0; k<_n; ++k){
                    _mat[k]->at(i, j)/=_sum[k];
                    sumr->at(k,i)+=_mat[k]->at(i, j);
                    sumc->at(k,j)+=_mat[k]->at(i, j);
                }   
            }    
        }
        

        for (i=0; i<_size; ++i){
            vi = i+1;
//            vi = i;
            for (k=0; k<_n; ++k){
                mr[k] += vi*sumr->at(k, i);
                mc[k] += vi*sumc->at(k, i);
            }
        }
        

        double i_ui2, j_uj2;
        for (i=0; i<_size; ++i){        
            vi = i+1;
//            vi = i;
            for (k=0; k<_n; ++k){
                i_ui2 = vi-mr[k];
                o_r[k] += (i_ui2*i_ui2)*sumr->at(k, i);
                
                j_uj2 = vi-mc[k];
                o_c[k] += (j_uj2*j_uj2)*sumc->at(k, i);
            }
        }
        
        for (k=0; k<_n; ++k){
            o_r[k] = std::sqrt(o_r[k]);
            o_c[k] = std::sqrt(o_c[k]);
        }
        
        /*calculate 6 measures of Haralick*/
        for (i=0; i<_size; ++i){
            for (j=0; j<_size; ++j){
                for (k=0; k<_n; ++k){
                    vi = i+1;
                    vj = j+1;
//                    vi = i;
//                    vj = j;

                    pij = _mat[k]->at(i,j);
                    i_j = vi - vj;

                    if (pij!=0.0){
                        _ene[k] += pij*pij;
                        _ent[k] += pij*(std::log(pij));
                        _con[k] += (i_j*i_j)*pij;
                        _hom[k] += pij/(1+std::abs(i_j));
                        if (o_r[k]!=0.0 && o_c[k]!=0.0)
                            _corr[k] += ((vi-mr[k])*(vj-mc[k])*pij)/(o_r[k]*o_c[k]);
                        
                        if (pij > _pobmax[k])
                            _pobmax[k] = pij;
                    }            
                }
            }
        }
        
        for (k=0; k<_n; ++k){
            _ent[k] *= (-1.0);
        }
            
        i = 0;
        
        //juntar as medidas estadistircas no vetor
        for (k=0; k<_n; ++k){
            fe->at(i) = _ene[k];i++;
            fe->at(i) = _ent[k];i++;
            fe->at(i) = _con[k];i++;
            fe->at(i) = _corr[k];i++;
            fe->at(i) = _hom[k];i++;
            fe->at(i) = _pobmax[k];i++;
        }
        
        
        for (k=0; k<_n; ++k){
            delete _mat[k];
        }
        delete _mat;

        delete []_sum;
        delete []_ene;
        delete []_ent;
        delete []_con;
        delete []_corr;
        delete []_hom;
        delete []_pobmax;

        delete sumr;
        delete sumc;

        delete []mr;
        delete []mc;
        delete []o_r;
        delete []o_c;
    }
};


/**
 * Descriptor BIC
 */
class BIC: public Descriptor{
protected:
    
public:
    
    BIC(){
        _size_features = 128;
        _name_desc = "BIC";
        _name_desc_short = "BIC";
        _id_desc = "1";
    }

    void compute(
        ImageIO *img,
        Vector *fe
    ){
        int i, j;
	unsigned char *data = reduceColor64(img);
        
        int half = int(fe->size()/2);
        int width = img->width();
        int height = img->height();
	for(i=1; i<height-1; i++) {
            for(j=1; j<width-1; j++) {
                unsigned char aux = data[i*width+j];
                      // se os 4 vizinhos e o  pixel central forem iguais....
                      if (  aux == data[(i-1)*width+j] && 
                            aux == data[(i+1)*width+j] && 
                            aux == data[i*width+(j-1)] && 
                            aux == data[i*width+(j+1)]   ){
                            // acumula no interior... na segunda metade do histograma
                            fe->at(half+aux)++;
                      }
                      else {                        
                            //acumular na borda
                            fe->at(aux)++;
                      }
                           
            }
	}
        delete data;
    }
};
#endif