cmake_minimum_required(VERSION 3.22)
#set(CMAKE_BUILD_TYPE Debug)
project(pca-reader LANGUAGES C)


#cmake -D IFFLAGE=OFF -D BUILD_SHARED_LIBS=OFF .

#unset(IFFLAGE CACHE)
#OPTION (IFFLAGE ON)
#message(${IFSTATIC})

set(PCA_READER_VAR "pca-reader loaded..." CACHE INTERNAL "")

add_library(${PROJECT_NAME} pca-reader.c include/pca-reader.h) #fd_reciever.c
if(IFFLAGE)
   message("flag on")
else()
   message("flag off")
endif()



#add_library(fd_reciever)
#set(CMAKE_BUILD_TYPE MinSizeRel)
#add_library(fd_reciever SHARED fd_reciever.c fd_reciever.h)
