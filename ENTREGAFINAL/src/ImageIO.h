/***
 * author: Liz Huancapaza H.
 **/

#ifndef IMAGEIO_H
#define	IMAGEIO_H
//namespace msw {

//#define cimg_display 0
//begin functions for draw Line anti-aliasing
#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((double)(X))+0.5))
#define fpart_(X) (((double)(X))-(double)ipart_(X))
#define rfpart_(X) (1.0-fpart_(X)) 
#define swap_(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)
//end

#include "CImg.h"

#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <string>
#include <cstdlib>
#include <deque>
#include <cassert>
#include <algorithm>
#include <set>

using namespace cimg_library;    

typedef unsigned int Pixel;
//typedef unsigned char Pixel;
typedef struct{
    Pixel R, G, B;
}ColorRGB;

ColorRGB     YELLOW_D = { 255,236,0 },
              WHITE_D = { 255,255,255 },
              GREEN_D = { 169,186,30 },
                RED_D = { 226,0,26 },
//               BLUE_D = { 0,0,255 },
               BLUE_D = { 0,134,203 },
            SKYBLUE_D = { 40,155,255 },
               PINK_D = { 255,100,255 },
             PURPLE_D = { 117,71,177},
             ORANGE_D = { 241,112,34},
              BLACK_D = { 0,0,0},
              CYAN_D = { 0,255,255};



typedef CImg<Pixel> Image;
const int x4[4] = {1, 0, -1,  0};
const int y4[4] = {0, 1,  0, -1};

const int x8[8] = {1, 1, 0, -1, -1, -1,  0,  1};
const int y8[8] = {0, 1, 1,  1,  0, -1, -1, -1};

class ImageIO{
private:  
    
    int *_xp;
    int *_yp;
    
    Image *_img;
    int _size;
    int _type; //3 => rgb; 1=> grayscale
    int _width;
    int _height;
    std::string _imgName;
    
    /**
     * @brief load the image from path.
     * @param path, file path for the image.
     */
    void makeBuffer(){
        _size = _img->width()*_img->height();
        _type = _img->spectrum();
        _width = _img->width();
        _height = _img->height();
        
        _xp = new int[_size];
        _yp = new int[_size];
        
        int x, y, i = 0;
        for(y  = 0; y < _height; ++y)
            for(x = 0; x < _width; x++){
                _xp[i] = x; 
                _yp[i] = y;
                
                i++;
            }        
    }

    std::string getFileName(std::string& path){
        std::string filename;        
        size_t found;
        found=path.find_last_of("/\\");
        filename = path.substr(found+1);
        return filename;
    }

public:
    
    
    /**
     * @brief Constructor.
     * @param imagepath
     */
    ImageIO(std::string imagepath){
        _img = new Image();
        _img->load(imagepath.c_str());
        _imgName = getFileName(imagepath);        
        makeBuffer();
    }
    
    /**
     * @brief Constructor.
     * @param width
     * @param height
     * @param type 1: grayscale, 3: rgb
     */
    ImageIO(int width, int height, int type){
        _img = new Image(width, height, 1, type, 0);
        makeBuffer();
    }

    /**
     * @brief Destructor.
     */
    ~ImageIO(){
        delete _img;
        delete []_xp;
        delete []_yp;
    }
    
    /**
     * @brief Return data of the image.
     * @return 
     */
    Pixel*& data(){
        return _img->_data;
    }

    /**
     * @brief Return 1 when is grayscale; 3 when is rgb.
     * @return 
     */
    int type(){
        return _type;
    }    

    /**
     * @brief Return the value of pixel.
     * @param x
     * @param y
     * @param id, (0,1,2) rgb or (0) grayscale
     * @return 
     */
    inline Pixel& at(int x, int y, int id){
//        assert(_width>x);
//        assert(_height>y);
        return _img->_data[id*_size+(y*_width+x)];
    }

    /**
     * @brief Return the value of pixel.
     * @param i, index of pixel.
     * @param id, [0,1,2]RGB and [0]GRAYSCALE
     * @return 
     */
    inline Pixel& at(int i, int id){
        return _img->_data[id*_size+i];
    }
    
    /**
     * @brief Return x value for each i pixel.
     * @param i
     * @return 
     */
    inline int x(int i){
        return _xp[i];
    }

    /**
     * @brief Return y value for each i pixel.
     * @param i
     * @return 
     */
    inline int y(int i){
        return _yp[i];
    }
    
    /**
     * @brief Return an array of grayscale values;
     * @return 
     */
    void gray(Pixel* gray){
        int i = 0;
        if(_type==1){
            for(i=0; i<_size; ++i){
                gray[i] = data()[i];
            }  
        }
        else if(_type==3){
            for(i=0; i<_size; ++i){
                gray[i] =       0.299 * data()[        i] +
                                0.587 * data()[  _size+i] +
                                0.114 * data()[2*_size+i];
            }
        }
    }
    
    void gray(int *gray){
        int i = 0;
        if(_type==1){
            for(i=0; i<_size; ++i){
                gray[i] = data()[i];
            }  
        }
        else if(_type==3){
            for(i=0; i<_size; ++i){
                gray[i] =      (0.299 * data()[        i] +
                                0.587 * data()[  _size+i] +
                                0.114 * data()[2*_size+i]);
            }
        }
    }
    
    /**
     * @brief Return an array to the red channel;
     * @return 
     */
    void getR(Pixel*& red_scale){
        assert(_type==3);        
        int i = 0;
        for(i=0; i<_size; ++i)
            red_scale[i] = data()[i];
    }
    
    /**
     * @brief Return an array to the green channel;
     * @return 
     */
    void getG(Pixel*& green_scale){
        assert(_type==3);
        int i = 0;
        for(i=0; i<_size; ++i)
            green_scale[i] = data()[_size+i];
    }
    
    /**
     * @brief Return an array to the blue channel;
     * @return 
     */
    void getB(Pixel*& blue_scale){
        assert(_type==3);
        int i = 0;
        for(i=0; i<_size; ++i)
            blue_scale[i] = data()[2*_size+i];
//            blue_scale[i] = _pixeldata[2*_size+i];
    }
    
    void getRGB(Pixel*& r, Pixel*& g, Pixel*& b){
        assert(_type==3);
        int i = 0;
        for(i=0; i<_size; ++i){
            r[i] = data()[i];            
            g[i] = data()[_size+i];
            b[i] = data()[2*_size+i];
        }
    }

    /**
     * @brief Convert to rgb.
     * @param path
     */
    void convert2rgb(){
        if(_type==1){
            Pixel* graybuff = new Pixel[_size];
            gray(graybuff);
            delete _img;
            _img = nullptr;
            _type = 3;
            _img = new Image(_width, _height, 1, _type, 0);
//            _pixeldata = _img->data();
            int i;
            for(i=0; i<_size; ++i){
                at(i, 0) = graybuff[i];
                at(i, 1) = graybuff[i];
                at(i, 2) = graybuff[i];
            }            
            delete []graybuff;
        }
    }

    /**
     * @brief Convert to gray.
     * @param path
     */
    void toGrayLuminance(){
        if(_type==3){
            Pixel* gray = new Pixel[_size];
            int i;
            for(i=0; i<_size; ++i){
                gray[i] =       0.299 * data()[        i] +
                                0.587 * data()[  _size+i] +
                                0.114 * data()[2*_size+i];
            }
            delete _img;
            _img = nullptr;
            _type = 1;
            _img = new Image(_width, _height, 1, _type, 0);

            for(i=0; i<_size; ++i){
                at(i, 0) = gray[i];
            }
            delete []gray;
        }
    }
    
    /**
     * @brief Save image as file.
     * @param path, where?
     */
    void save(std::string path){
        _img->save_png(path.c_str());
    }    
    
    /**
     * @brief Return the width of the image.
     * @return 
     */
    int width(){
        return _width;
    }

    /**
     * @brief Return the height of the image.
     * @return 
     */
    int height(){
        return _height;
    }

    /**
     * @brief Return the number of pixels.
     * @return 
     */
    int size(){
        return _size;
    }
    
    /**
     * @brief Update the name. 
     * @return 
     */
    std::string& name(){
        return  this->_imgName;
    }
    
    /**
     * @brief Display the image.
     */
    void display(){
        std::stringstream stm;
        stm<<this->width()<<" x "<<this->height();
        _img->display(stm.str().c_str(), false);
    }
    
    /**
     * @brief Return image cimg type;
     */
    Image* cimg(){
        return this->_img;
    }

};
#endif
