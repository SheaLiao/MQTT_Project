APPNAME=mqtt
LIBPATH=./lib/
INCPATH=./include/

CFLAGS+=-I${INCPATH}
CFLAGS+=-I ./openlibs/sqlite/install/include
CFLAGS+=-I ./openlibs/cJSON/install/include/cjson/
CFLAGS+=-I ./openlibs/mosquitto/install/include/ 
LDFLAGS+=-L${LIBPATH}
LDFLAGS+=-L ./openlibs/sqlite/install/lib 
LDFLAGS+=-L ./openlibs/cJSON/install/lib/
LDFLAGS+=-L ./openlibs/mosquitto/install/lib/ 

CC=gcc

all:
	${CC} ${CFLAGS} -g main.c -o ${APPNAME} ${LDFLAGS} -lmymqtt -lsqlite3 -lcjson -lmosquitto && export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/iot24/liaoshengli/mqtt_project/project/lib/


clean:
	rm -f ${APPNAME}

run:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBPATH}
