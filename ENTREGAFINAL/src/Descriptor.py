#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    author: Liz Maribel H. H.
"""
import imageio
import math
import cmath

def convert2gray(image):
    """
        Converte a imagem colorida em niveis de cinza
        Args: 
            image: imagem colorida
        Return:
            gray: array com os niveis de cinza
    """
    
    h=image.shape[0]
    w=image.shape[1]
    gray = [ [0 for j in range(w)] for i in range(h)]
    for row in range(h):
        for col in range(w):
            r = 0.299*float(image[row,col,0])
            g = 0.587*float(image[row,col,1])
            b = 0.114*float(image[row,col,2])
            gray[row][col] = int(r + g + b)
    return gray

def reduceColor64(img):
    """
        Reduce a cor da imagem em 64 cores
        Args: 
            img: imagem colorida
        Return:
            data: array contendo o id da cor
    """
    h=img.shape[0]
    w=img.shape[1]
    
    #numero de pixeis
    n = w*h
    
    #(2**b)**3 colores 
    b = 2
    
    #8 - 2 bits para criar a marcara de bits
    r = 8 - b
    
    #maskBit = 255
    maskBit = 0xFF
    #zerar os r primeiros bits da mascara
    maskBit = maskBit << r
    
    data = [ 0 for i in range(n)]
    for i in range (h):
        for j in range (w):
            idx = i*w+j
            C1 = (img[i,j,0] & maskBit)
            C2 = (img[i,j,1] & maskBit)
            C3 = (img[i,j,2] & maskBit)

            data[idx] = (C1>>2) | (C2>>4) | (C3>>6)
            
            #atualizar colores para a imagem
            #img[i,j,0] = C1
            #img[i,j,1] = C2
            #img[i,j,2] = C3

    return data

def BIC(img):
    """
        Implementacao do metodo Border Interior Classification
        Args:
            img: imagem
            fe: vetor que contenra os features
    """
    n = 128
    fe = [0.0 for i in range(n)]
    data = reduceColor64(img)
    #imageio.imwrite('process.png', img)
    half = int(n/2.0)
    h=img.shape[0]
    w=img.shape[1]


    for i in range(1, h-1):
        for j in range(1, w-1):
            aux = data[i*w+j]
            n1 = data[(i-1)*w+j]
            n2 = data[(i+1)*w+j]
            n3 = data[i*w+(j-1)]
            n4 = data[i*w+(j+1)]

            #if aux == 63:
                #print (aux)

            #// se os 4 vizinhos e o  pixel central forem iguais....
            if aux == n1 and aux == n2 and aux == n3 and aux == n4:
                #// acumula no interior... na segunda metade do histograma
                fe[half+aux] += 1.0                        
            else:
                
                #// acumula como borda...
                fe[aux] += 1.0
    return fe

def GLCooMatrix(dx, dy, img):
    """
        Implementacao do metodo Gray Level Co-ocorrence Matrix com as 6 medidas estadisticas do Haralick.
        Args:
            dx, dy: variacoes de deslocamento
            img: imagem
            fe: vetor que contenra os features
    """
    fe = []
    grayscales = convert2gray(img)
    
    _n = len(dy)
    _size = 256

    _mat = [ [ [0.0 for i in range(_size)] for j in range(_size)] for k in range(_n)]
    _sum = [0.0 for i in range(_n)]
    
    h=img.shape[0]
    w=img.shape[1]

    for x in range(w):
        for y in range(h):
            for i in range(_n):
                xj = int(dx[i]+x)
                yj = int(dy[i]+y)
                if xj>=0 and xj<w and yj>=0 and yj<h:
                    gi = int(grayscales[y][x])
                    gj = int(grayscales[yj][xj])
                    
                    _mat[i][gi][gj] += float(1.0)
                    _sum[i] += int(1)
    
    _ene  = [0.0 for i in range(_n)]
    _ent  = [0.0 for i in range(_n)]
    _con  = [0.0 for i in range(_n)]
    _corr = [0.0 for i in range(_n)]
    _hom  = [0.0 for i in range(_n)]
    _pobmax = [0.0 for i in range(_n)]

    mr = [0.0 for i in range(_n)]
    mc = [0.0 for i in range(_n)]
    o_r = [0.0 for i in range(_n)]
    o_c = [0.0 for i in range(_n)]
    
    sumr = [ [0.0 for j in range(256)] for i in range(_n)]
    sumc = [ [0.0 for j in range(256)] for i in range(_n)]
    
    #/*normalization*/
    for i in range(_size):
        for j in range(_size):
            for k in range(_n):
                _mat[k][i][j]/=float(_sum[k])
                sumr[k][i]+=float(_mat[k][i][j])
                sumc[k][j]+=float(_mat[k][i][j])


    #calcula mr e mc
    for i in range(_size):
        vi = i+1
        for k in range(_n):
            mr[k] += (vi)*sumr[k][i]
            mc[k] += (vi)*sumc[k][i]
    
    #calcula o_r e o_c
    for i in range(_size):
        vi = i+1
        for k in range(_n):
            o_r[k] += ((vi-mr[k])*(vi-mr[k]))*sumr[k][i]
            o_c[k] += ((vi-mc[k])*(vi-mc[k]))*sumc[k][i]
    
    
    #for k in range(_n):
        #o_r[k] = math.sqrt(o_r[k])
        #o_c[k] = math.sqrt(o_c[k])

    #/*calculate 6 measures of Haralick*/
    for i in range(_size):
        vi = i+1
        for j in range(_size):
            for k in range(_n):      
                vj = j+1
                pij = _mat[k][i][j]
                i_j = vi - vj
                if pij!=0.0:
                    _ene[k] += pij*pij
                    _ent[k] += pij*(math.log(pij)/math.log(2.0))
                    _con[k] += pij*(i_j*i_j)
                    _hom[k] += pij/(1+math.fabs(i_j))
                    if o_r[k]!=0.0 and o_c[k]!=0.0:
                        _corr[k] += ((vi-mr[k])*(vj-mc[k])*pij)/(o_r[k]*o_c[k])
                    if pij > _pobmax[k]:
                        _pobmax[k] = float(pij)

    for k in range(_n):
        _ent[k] *= -1.0
        
    i = 0
    for k in range(_n):
        fe.append(_ene[k])
        fe.append(_ent[k])
        fe.append(_con[k])
        fe.append(_corr[k])
        fe.append(_hom[k])
        fe.append(_pobmax[k])
        
    return fe
