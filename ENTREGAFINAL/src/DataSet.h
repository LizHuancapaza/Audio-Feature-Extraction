/***
 * author: Liz Huancapaza H.
 **/

#ifndef DATASET_H
#define	DATASET_H

//#include "../engine/Util.h"
#include <cassert>
#include <iostream>
#include <fstream>      // std::ifstream
#include <vector>
#include <cstdlib>
//#include <string>
#include <cstring>
#include <sstream>      // std::istringstream
#include <string> 

//using namespace std;
class DataSet{
protected:
    const char *_file_dataset;
    
    const char *_name_dataset;
    const char *_name_dataset_short;
    
    const char *_path_sources;
    const char *_path_results;
    
    const char *_class_labels;
    int _classes;
    int _rows;

    std::vector<std::vector<std::string> > _data;

public:    
    auto data()->std::vector<std::vector<std::string> >
    {
        return this->_data;
    }
    
    int instances(){
        return this->_rows;
    }
    
    int classes(){
        return this->_classes;
    }
    
    const char *classesLabels(){
        return this->_class_labels;
    }
    const char *fileDataSet(){
        return this->_file_dataset;
    }
    const char *nameDataSet(){
        return this->_name_dataset;
    }
    const char *nameDataSetShort(){
        return this->_name_dataset_short;
    }
    const char *pathSources(){
        return this->_path_sources;
    }
    const char *pathResults(){
//        std::cout<<"x11"<<std::endl;
        return this->_path_results;
    }
    
    char* string_as_array(std::string str){
        char *y = new char[str.length() + 1]; // or
        std::strcpy(y, str.c_str());
        return y;
    }

    DataSet(std::string _source, std::string _result){
        std::string _file(_source+"data.csv");
        
        _file_dataset = _file.c_str();
        
        _path_sources = string_as_array(_source);
        _path_results = string_as_array(_result);
        
        std::ifstream inputf(_file_dataset);
        std::string token;
        std::string line;

        if(std::getline(inputf, line, '\n')){
            _name_dataset = string_as_array(line);
        }

        if(std::getline(inputf, line, '\n')){
            _name_dataset_short = string_as_array(line);
        }

        if(std::getline(inputf, line, '\n')){
            _rows = std::stoi(line);
        }

        if(std::getline(inputf, line, '\n')){
            _classes = std::stoi(line);
        }
        
        if(std::getline(inputf, line, '\n')){
            _class_labels = string_as_array(line);
        }
        
        for(line; getline( inputf, line); ){
            std::istringstream iss(line);
            std::vector<std::string> linedat;
            while (getline(iss, token, ',')){
                linedat.push_back(token);
            }
            _data.push_back(linedat);
        }
    }
};
#endif