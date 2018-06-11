
# <h1> Projeto para Processamento de imagem
______________________________________________________________________________

Nome: Liz Huancapaza Hilasaca

N.USP: 10423932

# <h2> Projeto: Extração de caracterı́sticas para classificação de áudios.
   O objetivo deste projeto é implementar, comparar e conhecer as vantagens dos métodos existentes para descrever características baseadas em cor e textura para processar imagens geradas desde arquivos de audio .wav.
______________________________________________________________________________

Imagens: 

Imagens de Espectrogramas no formato RGB gerados apartir dos áudios. Este é um conjunto de arquivos de áudio .wav, cada um contendo uma única palavra falada em inglês. Estas palavras são faladas por uma variedade de falantes diferentes. O conjunto de dados tem aproximadamente 65000 audios, organizados em pastas por palavras: "Yes", "No", "Up", "Down", "Left", "Right", "On", "Off", "Stop", "Go", "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", and "Nine". Dataset [link aqui!](http://download.tensorflow.org/data/speech_commands_v0.01.tar.gz).



![Espectrograma gerado a partir do audio 0](/imagens/2aa787cf_nohash_0.png)
Format: ![Alt Text](url)

![Espectrograma gerado a partir do audio 1](/imagens/2aa787cf_nohash_1.png)
Format: ![Alt Text](url)













Imagens de database Corel-1k

______________________________________________________________________________

Etapas:

1. Gerar os Espectrogramas (Imagens): Para gerar os espectrogramas vou empregar a transformada de furier e escalas logarı́tmicas para melhorar a visualização dos detalhes do sonido (palavra falada) no espectrograma. Depois teremos como resultado uma imagem com formato: .png 

2. Empregar métodos descritores de cor e textura.
    * Descritores de cor
    
        * Border/Interior Classification (BIC)
    * Descritores de textura
    
        * Co-ocurrence Matrix
3. Finalmente para medir a qualidade empregarei a acurácia de classificação.
