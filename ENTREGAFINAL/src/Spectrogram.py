#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    author: Liz Maribel H. H.
"""
#import soundfile as sf
import numpy as np
from scipy.signal import spectrogram
import librosa
import matplotlib.pyplot as plt
import scipy
import os

def log_melspectrogram(inputfile, outfile):
    """
    Metodo que cria espectrogramas do tipo log melspectrogram
    Args:
        inputfile   : wav arquivo de audio
        outfile     : arquivo onde sera guardado o spectrogram
    """
    out_rate = 22050    
    frames, rate = librosa.load(inputfile, sr=out_rate, mono=True)
    audio_data = frames
    samplerate = rate 
    n_mels=32
    n_fft=512
    hop_length=160
    
    spec_width = 100
    spec_height = 100
    speccmap = plt.cm.inferno#biennn
    #speccmap = plt.cm.jet
    #speccmap = plt.cm.rainbow


    spectrogram = librosa.feature.melspectrogram(audio_data, sr=samplerate, n_mels=n_mels, n_fft=n_fft, hop_length=hop_length)

    # convert to log scale (dB)
    log_spectrogram = librosa.amplitude_to_db(spectrogram, ref=np.max)
    
    specMatrixDB1 = np.flipud(log_spectrogram)
    norm = plt.Normalize(vmin=specMatrixDB1.min(), vmax=specMatrixDB1.max())
    image = speccmap(norm(specMatrixDB1))        
    newImage = scipy.misc.imresize(image, (spec_height, spec_width))
    plt.imsave(outfile, newImage)
        
    #return 

#def make_spectogram(inputfile, outfile):
    #frames, rate = sf.read(inputfile)
    ## Pego um canal apenas
    #audioChannel = 0
    #if (len(np.shape(frames)) == 1):
        #selChannel = frames
    #else:
        #if (audioChannel == 0):
            #selChannel = frames[:,0]
        #else:
            #selChannel = frames[:,1]

    ## Scipy
    #freqList, t, Sxx = spectrogram(x=selChannel, fs=rate, nfft=512)
    #ref = 1 # 32768 for int16 and 1 for float
    #specDB = 20 * np.log10((Sxx/ref)+0.001)
    ##specDB = 20 * np.log10((Sxx/ref))
    #specPWR = np.power(specDB, 2) 

    #specDB = np.flipud(specDB)
    #specPWR = np.flipud(specPWR)


    ## Librosa
    #spec_width = 250
    #spec_height = 250

    ##speccmap = plt.cm.magma#
    ##speccmap = plt.cm.nipy_spectral#
    ##speccmap = plt.cm.gist_ncar#
    ##speccmap = plt.cm.hsv#
    ##speccmap = plt.cm.viridis#
    ##speccmap = plt.cm.tab20#
    ##speccmap = plt.cm.Dark2#
    #speccmap = plt.cm.inferno#biennn
    ##speccmap = plt.cm.jet#biennn
    ##speccmap = plt.cm.terrain
    ##speccmap = plt.cm.gist_ncar
    #spectro = librosa.stft(selChannel, n_fft=512, window=scipy.signal.blackmanharris)
    ##print (spectro)
    ##spectro = np.real(spectro)
    #specMatrixDB = librosa.amplitude_to_db(spectro, ref=np.max, top_db=50)
    #specMatrixDB1 = np.flipud(specMatrixDB)
    #norm = plt.Normalize(vmin=specMatrixDB1.min(), vmax=specMatrixDB1.max())
    #image = speccmap(norm(specMatrixDB1))        
    #newImage = scipy.misc.imresize(image, (spec_height, spec_width))
    #plt.imsave(outfile, newImage)

def make_spectograms(fileslist, dirinput, dirouput):
    """
    make spectrogram from the given audio files
    Args:
        inputfile: file list
        dirinput: path of the dir where are the files
        dirouput: path of the dir where save the spectrograms.
    """
    pathsounds = fileslist
    f=open(pathsounds)
    lines=f.readlines()
    classes = lines[4]
    classes = classes.strip()
    classes = classes.split(';')
    for c in classes:
        d = dirouput+c
        if not os.path.exists(d):
            os.makedirs(d)
        print(d)
        
    #exit()
    for i in range(5, len(lines)):
        #print (lines[i])
        line = lines[i].strip()
        line = line.split(',')
        name = os.path.basename(line[0])
        name = os.path.splitext(name)
        name = name[0]
        #print (name)
        inputfile = dirinput+line[0]
        outfile = dirouput+classes[int(line[1])]+"/"+name+".png" 
        print(outfile)
        log_melspectrogram(inputfile, outfile)
        
#inicio
if __name__ == "__main__":

    make_spectograms("./input/audio/training_go_stop.csv",
                    "./input/audio/", 
                    "./input/spectromams/go_stop/")
