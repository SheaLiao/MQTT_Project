APPNAME=mqtt
LIBPATH=./lib/
INCPATH=./include/

CFLAGS+=-I${INCPATH}
CFLAGS+=-I ./openlibs/cJSON/install/include/cjson/
CFLAGS+=-I ./openlibs/mosquitto/install/include/ 
CFLAGS+=-I ./openlibs/iniparser/install/iniparser/ 
LDFLAGS+=-L${LIBPATH}
LDFLAGS+=-L ./openlibs/cJSON/install/lib/
LDFLAGS+=-L ./openlibs/mosquitto/install/lib/ 
LDFLAGS+=-L ./openlibs/iniparser/install/lib/ 

CC=gcc

all:
	${CC} ${CFLAGS} -g main.c -o ${APPNAME} ${LDFLAGS} -lmymqtt -liniparser -lcjson -lmosquitto && export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/iot24/liaoshengli/mqtt_project/project/lib/


clean:
	rm -f ${APPNAME}

run:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBPATH} && ./${APPNAME} -d
	
