#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    author: Liz Maribel H. H.
"""
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
from sklearn.cross_validation import StratifiedShuffleSplit
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn import metrics
from sklearn.cross_validation import StratifiedKFold
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.svm import LinearSVC

from sklearn import cross_validation
from sklearn.cross_validation import cross_val_score

import glob
import sys
import itertools 

#from boxPlot import *

class DataFeature():
    """
    Classe para leer o arquivo de dados de features
    
    """
    def __init__(self,filename):
        f = open(filename)    
        # name dataset
        self.ndataset = f.readline()
        # name dataset short
        self.ndataset_short = f.readline()
        # name feature descriptor
        self.ndataset_desc = f.readline()
        # name feature descriptor short
        self.ndataset_desc_short = f.readline()
        # name feature descriptor short
        self.ndataset_desc_id = f.readline()
        
        # rows
        self.rows = int(f.readline())
        # cols
        self.cols = int(f.readline())
        # n classes
        zn = f.readline()
        self.z = [[] for x in range(int(zn))]
        # labels classes
        self.labelclasses = []
        for ln in f.readline().split(";"):
            self.labelclasses.append(ln)
        # features
        self.labelcols = []
        for ln in f.readline().split(";"):
            self.labelcols.append(ln)
        
        self.w = []
        self.X = []
        self.y = []

        for i in range(0, self.rows):
            sline = f.readline().split(";")
            self.w.append(int(sline[0]))
            self.y.append(int(sline[self.cols+1]))
            self.z[self.y[i]].append(int(i))
            matrixrow = []
            for j in range(1, self.cols+1):
                matrixrow.append(float(sline[j]))
            self.X.append(matrixrow)

        f.close()
        
    def gNameDataSet(self):
        return self.ndataset.strip()
    
    def gNameDataSetShort(self):
        return self.ndataset_short.strip()
    
    def gNameFeatureDescriptor(self):
        return self.ndataset_desc.strip()
    
    def gNameFeatureDescriptorShort(self):
        return self.ndataset_desc_short.strip()
    
    def gNameFeatureDescriptorID(self):
        return self.ndataset_desc_id.strip()
        
    def g_rows(self):
        return self.rows

    def g_cols(self):
        return self.cols

    def g_z(self):
        return self.z

    def g_y(self):
        return self.y

    def g_w(self):
        return self.w

    def g_X(self):
        return self.X

    def LabelFeatures(self):
        return self.labelcols    

    def LabelClasses(self):
        return self.labelclasses


def plotghist(titel, ytitle, clas, desc, data, filesave):
    """
    metodo para salvar os resultados de accuracia como histogramas
    Args:
        titel: titulo
        ytitle: titulo para o eixo y
        clas: informacao dos classificadores
        desc: informacao dos descritores
        data: matrix com resultados
        filesave: nome do arquivo para salvar a imagem.
    """

    print (clas)
    print (desc)
    print (data)

    # Setting the positions and width for the bars
    pos = list(range(len(data[0])))
    width = 0.15

    # Plotting the bars
    fig, ax = plt.subplots(figsize=(7,5))
    
    #colorsa = ["windows blue", "amber", "greyish", "faded green", "dusty purple"]
    #pal = sns.palplot(sns.xkcd_palette(colorsa))
    
    #pal = sns.color_palette('Paired', len(data))
    #pal = sns.color_palette('muted', len(data))
    colorsd = {'#ff0000','#00a3f8','#46c83c','#efcd31','#9467bd','#6e6e6e','#ff7f0e'}
    pal = itertools.cycle(colorsd)
    
    #pal = sns.color_palette('bright', len(data))
    pal = ['#ff0000','#00a3f8','#46c83c','#efcd31','#9467bd','#6e6e6e','#ff7f0e']
    
    #colors = pal.as_hex()
    colors = pal
    #colors = ["#904aac", "#f24c34", "#42ade3", "#fd9a00", "#009e87","#ff6a9e"]
    #colors = ["#9b59b6", "#3498db", "#95a5a6", "#e74c3c", "#34495e", "#2ecc71"]
    print ("sssssssssssssssssssssssssssssssssss",clas)
    print ("sssssssssssssssssssssssssssssssssss",len(data))
    for i in range(0, len(data)):
            # Create a bar with mid_score data,
            # in position pos + some width buffer,
            if i == 0:
                    pss = pos
            else:
                    pss = [p + (width*i) for p in pos]

            plt.bar(pss,
                    #using df['mid_score'] data,
                    data[i],
                    # of width
                    width,
                    # with alpha 0.5
                    alpha=0.5,
                    # with color
                    #color=colors.cycle().next(),
                    color=colors[i],
                    # with label the second value in first_name
                    label=clas[i])

    # Set the y axis label
    ax.set_ylabel(ytitle)

    # Set the chart's title
    ax.set_title(titel)

    # Set the position of the x ticks
    ax.set_xticks([p + (len(clas)/2.0) * width - (width/2.0) for p in pos])

    # Set the labels for the x ticks
    ax.set_xticklabels(desc)

    # Setting the x-axis and y-axis limits
    plt.xlim(min(pos)-width, max(pos)+width*5)
    #va=0
    #for i in range(1, len(df)):
            #va=va+df[i]
    #plt.ylim([0, max(va)] )
    plt.ylim([0, 1.0] )

    # Adding the legend and showing the plot
    plt.legend(clas, loc='lower right')
    plt.grid()

    plt.savefig(filesave, dpi=300, bbox_inches='tight',pad_inches=0)

    plt.close()

#calculate accuracy: cm: confusion matrix
def acuracy(cm):
    """
    metodo que calcula a accuracy do resultado.
    Args:
        cm: confusion matrix
    """

    smii = 0.0
    smij = 0.0
    for i in range(0, len(cm[0])):
        smii = smii+cm[i][i]
    
    for i in range(0, len(cm[0])):
        for j in range(0, len(cm[0])):
            smij = smij+cm[i][j]
            
    return smii/smij



def classiff(DF, model, skf):  
    """
    metodo para realizar a classificacao de um determinado dataset
    Args:
        DF: data matrix
        model: classifcador
        skf: ids do dataset divididos em teste e treino
    """

    w = DF.g_w()
    X = DF.g_X()
    y = DF.g_y()
    z = DF.g_z()
        
    Xt = pd.DataFrame(X).as_matrix()
    yt = np.asarray(y)
    
    scores = []
    for train_index, test_index in skf:
        X_train, X_test = Xt[train_index], Xt[test_index]
        y_train, y_test = yt[train_index], yt[test_index]  
  
        model.fit(X_train, y_train)
        expected = y_test
        predicted = model.predict(X_test)
        score = model.score(X_test, y_test)

        cm = metrics.confusion_matrix(expected, predicted)
        acc = acuracy(cm)
        
        scores.append(score)
        classes = [i for i in range(0, len(z))]
                
    scorest = np.array(scores)  
    resm = scorest.mean()

    #arrayscores.append(resm)
    print(fiph)
    #print(scorest)
    print("ac: "+str(resm))
    #print("")        

    return resm, scores


if __name__ == "__main__":
    """
    metodo de inicio para realizar a classificacao
    Args:
        sys.argv[1]: path onde estao os arquivos .data dos features
    """

    configfiles = glob.glob(sys.argv[1]+'*.data')
    
    arrayscores = []  
    arrayscoresall = []  
    arraysnames = []  

    for filex in configfiles:
        DF =  DataFeature(filex)
        fiph = DF.gNameFeatureDescriptorShort()
        
        skf = StratifiedKFold(DF.g_y(), n_folds=10)

        #skf = StratifiedShuffleSplit(DF.g_y(), n_iter=1, test_size=float(testsm), random_state=0)
        #skf = StratifiedShuffleSplit(DF.g_y(), n_iter=1, test_size=0.3, random_state=0)

        arraymodel = []
        arraymodel.append(RandomForestClassifier(random_state=0))
        #arraymodel.append(RandomForestClassifier(n_estimators=100, random_state=0))
        arraymodel.append(GaussianNB())
        arraymodel.append(KNeighborsClassifier(n_neighbors=1))
        arraymodel.append(SVC(class_weight='balanced'))
        
        res = []
        rescs = []
        for model in arraymodel:
            #scores = cross_val_score(model, DF.g_X(), DF.g_y(), cv=10, scoring='accuracy')
            #print(scores.mean())
            
            re, scs  = classiff(DF, model, skf)
            res.append(re)
            rescs.append(scs)
            
        arrayscores.append(res)
        arrayscoresall.append(rescs)
        
        na = DF.gNameFeatureDescriptorShort()
        arraysnames.append(na)

    #prepara para visualizar os resultados de accuracy em histogramas
    fd = [[0 for x in range(0,len(arrayscores))] for y in range(0, len(arrayscores[0]))] 
    for i in range(0, len(arrayscores)):
        for j in range(0, len(arrayscores[0])):
            fd[j][i] = arrayscores[i][j]

    #Save results
    ytile = 'Accuracy'
    titel = '10-k-flod'
    
    clas = ['RF','GNB','KNN','SVM']
    desc = arraysnames
    
    #salvar resultados em arquivos
    outda = []
    for i in range(0, len(fd[0])):
        strs = desc[i]+" & &"
        for c in range(0, len(clas)):
            strs = strs+str(format(fd[c][i]*100.0, '.2f'))+"&"
        strs = strs+""" \\\ \hline"""
        outda.append(strs)
    
    thefile = open(sys.argv[1]+'outclass.txt', 'w')
    for item in outda:
        thefile.write("%s\n" % item)
        
    #salvar histogramas de accuracies
    plotghist(titel, ytile, clas, desc, fd, sys.argv[1]+"1.png")

