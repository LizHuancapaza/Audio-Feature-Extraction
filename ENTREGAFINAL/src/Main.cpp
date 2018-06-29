/***
 * author: Liz Huancapaza H.
 **/

#include "Descriptor.h"
#include "ImageIO.h"
#include "DataSet.h"

int main(int argc, char* *argv) {
    int i=0, j;
    std::string pathinput(argv[1]);
    std::string pathoutput(argv[2]);

    DataSet *datRef = new DataSet(pathinput, pathoutput);
    
    //GLCooMatrix
    std::vector<int> dx = {0,0,0,1,3,5};
    std::vector<int> dy = {1,3,5,0,0,0};
    CoocMatrix *glcm = new CoocMatrix(dx, dy);
    VectorMatrix *vm_glcm = new VectorMatrix(datRef->instances(), glcm->size());
    
    for (auto ins : datRef->data()){
        std::cout<<ins[0]<<":"<<ins[1]<<std::endl;
        ImageIO *img = new ImageIO(pathinput+ins[0]);
        glcm->compute(img, vm_glcm->at(i));
        vm_glcm->classe(i) = std::stoi(ins[1]);
        delete img;
        i++;
    }
    vm_glcm->normalizationColumnsZScore(0, vm_glcm->cols());
    vm_glcm->write(pathoutput+glcm->shortName()+"_features.data", datRef, glcm->name(), glcm->shortName(), glcm->id());
    delete glcm;


    //BIC descriptor
    BIC *bic = new BIC();
    VectorMatrix *vm_bic = new VectorMatrix(datRef->instances(), bic->size());
    i=0;
    for (auto ins : datRef->data()){
        std::cout<<ins[0]<<":"<<ins[1]<<std::endl;
        ImageIO *img = new ImageIO(pathinput+ins[0]);
        bic->compute(img, vm_bic->at(i));
        vm_bic->classe(i) = std::stoi(ins[1]);
        delete img;
        i++;
    }
    vm_bic->normalizationColumnsZScore(0, vm_bic->cols());
    vm_bic->write(pathoutput+bic->shortName()+"_features.data", datRef, bic->name(), bic->shortName(), bic->id());
    delete bic;

    
    //mix GLCM + BIC
    int nrows = vm_glcm->rows();
    int ncols = vm_glcm->cols()+vm_bic->cols();
    VectorMatrix *vm_mix = new VectorMatrix(nrows, ncols);
    std::string name = "GLCM+BIC";
    const char *names = name.c_str();
    const char *idmix = "3";
    for (i=0; i<vm_mix->rows(); ++i){
        for (j=0; j<vm_glcm->cols(); ++j){
            vm_mix->at(i, j) = vm_glcm->at(i, j);
        }
        
        for (j=0; j<vm_bic->cols(); ++j){
            vm_mix->at(i, (j+vm_glcm->cols())) = vm_bic->at(i, j);
        }
        vm_mix->classe(i) = vm_bic->classe(i);
    }
    vm_mix->write(pathoutput+name+"_mix_features.data", datRef, names, names, idmix);
    delete vm_bic;    
    delete vm_glcm;
    delete vm_mix;
    
    return 0;
}