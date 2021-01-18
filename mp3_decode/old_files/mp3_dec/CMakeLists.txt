cmake_minimum_required(VERSION 3.17)
project(mp3_dec C)

set(CMAKE_C_STANDARD 99)


set(PROJECT_HEADERS
        MP3Decoder.h
        )

set(PROJECT_SOURCES
        main.c
        MP3Decoder.c
        )

#include_directories(C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix/pub)

#link_directories(C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix/libhelix.a)

add_executable(mp3_dec ${PROJECT_SOURCES} ${PROJECT_HEADERS})


#include_directories(lib/helix)

#link_directories(C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix)


find_library(HELIX helix C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix)
#target_link_libraries(mp3_dec LINK_PUBLIC ${HELIX})
#target_link_libraries(mp3_dec LINK_PUBLIC ${HELIX})

#target_link_libraries(mp3_dec PUBLIC C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix/libhelix.a)
#link_directories(C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib)
#target_link_libraries(mp3_dec PUBLIC libhelix.a)
#LINK_LIBRARIES(C:/Github_Repos/labo-de-micros-tpf/mp3_decode/mp3_dec/lib/helix)