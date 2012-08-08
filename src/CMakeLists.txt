CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES (${ImageMagick_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${Animal_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${Scilab_INCLUDE_DIRS})

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

SET (SRC_FILES 
    analysis_int.c 
    canny_int.c
    deskew_int.c
    dewarp_int.c
    hello_int.c
    imfinfo.c
    img_int.c
	imread.c 
    imvariance_int.c 
    imwrite.c 
    libsip.c
    misc_int.c
    mogrify_int.c
	morphology_int.c 
    segment_int.c 
    sip_common.c 
    unwrapl_int.c 
    unwrapp_int.c
    )

ADD_LIBRARY(sip SHARED ${SRC_FILES})
