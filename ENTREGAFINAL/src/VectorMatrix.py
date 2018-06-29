#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    author: Liz Maribel H. H.
"""
class Vector:
    """
        Classe que define um vetor de carateristicas
    """

    def __init__(self, size):
        self._size = size
        self._vec = [0.0 for i in range(size)]

    def getV(self, i):
        return self._vec[i]

    def setV(self, i, v):
        self._vec[i] = v

    def inprint(self):
        for i in range(self._size):
            print(self._vec[i], end=' ')


class VectorMatrix:
    """
        Classe que define uma matriz de vetor de carateristicas
    """

    def __init__(self, rows, cols):
        self._mat = [ Vector(cols) for j in range(r)]
        self._rows = rows
        self._cols = cols

    def getV(self, row, col):
        return self._mat[row].get(col)
    
    def setV(self, row, col, val):
        self._mat[row].setV(col, val)
