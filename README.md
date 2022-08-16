﻿# PhotoBatch
[USO]: PhotoBatch --[Rename|Scale|Convert|Resize] [Opções]
    
    O PhotoBatch possui quatro modos de operação. Somente um modo pode estar ativo. 
    Para ativar um modo, use uma das seguintes flags: 	
    
    - Rename   :  Ativa o modo para renomear arquivos.
    - Scale    :  Ativa o modo para converter arquivos.
    - Convert  :  Ativa o modo para redimensionar arquivos.
    - Resize   :  Ativa o modo para escalar arquivos.

    As seguintes opções são comuns a todos os modos: 
    - Folder   : Especifica a pasta onde o PhotoBatch irá atuar.
    - Filter   : Especifica o filtro usado para selecionar arquivos.


    Opções do modo Rename: 
    No modo Rename, cada arquivo selecionado será renomeado para o formato
    <Prefix>_<N>.<ext>, onde <Prefix> e <N> são especificados através de opções. 
    <ext> é a extensão do arquivo que permance inalterada. 

    --Prefix=<Prefix>  : Prefixo usado para renomear os arquivos.
    --StartNumber=<N>  : Número inicial a ser adicionado em cada arquivo que for renomeado.


    Opções do modo Convert: 
    No modo Convert, cada arquivo selecionado será convertido do formato <from> para o formato <to>.
    <from> e <to> não podem ser iguais. O nome do arquivo permance inalterado. 

    --From<jpg|png>  : Extensão de origem para conversão. 
    --To=<jpg|png>   : Extensão de destino para conversão.


	Opções do modo Resize: 
    No modo Resize, cada arquivo selecionado será redimensionado para as dimensões especificadas nas seguintes opções. Note que N >= 1. 

    --Width=<N>  : Novo comprimento da imagem.
    --Height=<N> : Nova altura da imagem.

    Opções do modo Scale: 
    No modo Scale, cada arquivo selecionado será escalado uniformemente pelo fato especificado pelas seguintes opções.
    Note que N > 0 e será interpretado como um número em ponto flutuante. 

   --Amount=<N>  : Fato de escala a ser aplicado em cada imagem selecionada.

    Exemplos de uso: 
	
    Renomear todos os arquivos da pasta "C:\Viagem" com a extensão ".png" para Viagem_N_.png, onde N começa do 0. 
    PhotoBatch --Rename --Folder="C:\Viagem" --Filter=".png" --StartNumber=0

    Redimensionar todos os arquivos na pasta "C:\Viagem" com a extensão ".jpg" para o tamanho 500x500. 
    PhotoBatch --Resize --Folder="C:\Viagem" --Filter=".jpg" --Width=500 --Height=500

    Converter todos os arquivos da pasta "C:\Viagem" de "png" para "jpg". 
    PhotoBatch --Convert --Folder="C:\Viagem" --From=png --To=jpg

    Reduzir pela metade o tamanho de todos os arquivos "png" da pasta "C:\Viagem"
    PhotoBatch --Scale --Folder="C:\Viagem" --Filter=".png" --Amount=0.5
