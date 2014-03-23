Looper
======

 An awsome project made By the geniuses LABY 'Poutnic' Bastien and FOUCHIER 'iNeken' Florent. It's all about music. "Don't call me 'sir'. Call me 'sir-vivor'" -Lil' Wayne
 
Installation (linux only)

  Move to the cloning directory path :
  > cd cloning_path/Looper/
  
  If you don't have premake4, install it :
  > sudo apt-get install premake4
  
  Generate makefile :
  > premake4 gmake
  
  Compile and link :
  > make
  
  This is a temporary issue to solve some library linking :
  > export LD_LIBRARY_PATH=cloning_path/Looper/lib/fmod/lib/:$LD_LIBRARY_PATH 
  
  > export LD_LIBRARY_PATH=cloning_path/Looper/lib/opencv2/lib/:$LD_LIBRARY_PATH
  
  Run the project :
  > ./Animusd
