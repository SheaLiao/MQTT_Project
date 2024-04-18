LIBNAME=mymqtt
INSTPATH=`pwd`/../lib/ 
INCPATH=`pwd`/../include/ 
SQLPATH=`pwd`/../openlibs/sqlite/install/include/ 
JSONPATH=`pwd`/../openlibs/cJSON/install/include/cjson/
MOSQPATH=`pwd`/../openlibs/mosquitto/install/include/
INIPATH=`pwd`/../openlibs/iniparser/install/iniparser

CFLAGS+=-I${INCPATH}
CFLAGS+=-I${SQLPATH}
CFLAGS+=-I${JSONPATH}
CFLAGS+=-I${MOSQPATH}
CFLAGS+=-I${INIPATH}

CC=gcc
AR=ar 

all: dynamic_lib static_lib
	@make install
	@make clear

dynamic_lib:
	${CC} -shared -fPIC *.c -o lib${LIBNAME}.so ${CFLAGS}

static_lib:
	${CC} -c *.c ${CFLAGS}
	${AR} -rcs lib${LIBNAME}.a *.o

install:
	cp -rf lib${LIBNAME}.* ${INSTPATH}

uninstall:
	rm -rf ${INSTPATH}/lib${LIBNAME}.*

clear:
	rm -f *.o
	rm -f lib${LIBNAME}.*
