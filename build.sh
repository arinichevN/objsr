#!/bin/bash

APP=objsr
APP_DBG=`printf "%s_dbg" "$APP"`
INST_DIR=/usr/sbin
CONF_DIR=/etc/controller
CONF_DIR_APP=$CONF_DIR/$APP

#DEBUG_PARAM="-Wall -pedantic"
DEBUG_PARAM="-Wall -pedantic -g"
MODE_DEBUG=-DMODE_DEBUG
MODE_FULL=-DMODE_FULL

NONE=-DNONEANDNOTHING

function move_bin {
	([ -d $INST_DIR ] || mkdir $INST_DIR) && \
	cp $APP $INST_DIR/$APP && \
	chmod a+x $INST_DIR/$APP && \
	chmod og-w $INST_DIR/$APP && \
	echo "Your $APP executable file: $INST_DIR/$APP";
}

function move_bin_dbg {
	([ -d $INST_DIR ] || mkdir $INST_DIR) && \
	cp $APP_DBG $INST_DIR/$APP_DBG && \
	chmod a+x $INST_DIR/$APP_DBG && \
	chmod og-w $INST_DIR/$APP_DBG && \
	echo "Your $APP executable file for debugging: $INST_DIR/$APP_DBG";
}

function move_conf {
	([ -d $CONF_DIR ] || mkdir $CONF_DIR) && \
	([ -d $CONF_DIR_APP ] || mkdir $CONF_DIR_APP) && \
	cp -r ./db/data.db $CONF_DIR_APP && \
	chmod -R a+rw $CONF_DIR_APP
	echo "Your $APP configuration files are here: $CONF_DIR_APP";
}

#your application will run on OS startup
function conf_autostart {
	cp -v init.sh /etc/init.d/$APP && \
	chmod 755 /etc/init.d/$APP && \
	update-rc.d -f $APP remove && \
	update-rc.d $APP defaults 30 && \
	echo "Autostart configured";
}
#clear && clear &&  gcc -DMODE_DEBUG -c main.c -D_REENTRANT -Wall -pedantic -g

function build_lib {
	cd lib  && \
	local da=(app util timef serial dbl)
	for d in "${da[@]}"
	do
		gcc $1 $3 -c ${d}.c -D_REENTRANT $DEBUG_PARAM -lpthread
	done
	cd ../ && \
	for d in "${da[@]}"
	do
		ar -crvs libpac.a lib/${d}.o 
	done
}

function build_acp {
	cd lib/ACP && \
	gcc $1 $3 -c ACP.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd serial && \
	gcc $1 $3 -c ACPSerial.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c ACPSerialPortParam.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../TCP && \
	gcc $1 $3 -c ACPTCP.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../../../ && \
	ar -crvs libpac.a lib/ACP/ACP.o lib/ACP/serial/ACPSerial.o lib/ACP/serial/ACPSerialPortParam.o lib/ACP/TCP/ACPTCP.o
}

function build_acp_serial_server {
	cd lib/ACP/serial/server
	local da=(ACPSS ACPSSPort)
	for d in "${da[@]}"
	do
		gcc $1 $3 -c ${d}.c -D_REENTRANT $DEBUG_PARAM -lpthread
	done
	cd ../../../../ && \
	for d in "${da[@]}"
	do
		ar -crvs libpac.a lib/ACP/serial/server/${d}.o 
	done
}

function build_sqlite {
	if [ ! -f sqlite3.o ]; then
	gcc $1 $3 -DSQLITE_THREADSAFE=2 -DSQLITE_OMIT_LOAD_EXTENSION  -c sqlite3.c -D_REENTRANT $DEBUG_PARAM -lpthread
	fi
	ar -crvs libpac.a sqlite3.o
}

function build_model {
	cd model  && \
	cd EM  && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c param.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../  && \
	cd Matter  && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c param.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../  && \
	cd Sensor  && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c param.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../  && \
	cd Channel
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c llistm.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c param.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd ../  && \
	cd ../  && \
	ar -crvs libpac.a model/Channel/main.o model/Channel/main.o model/Channel/llistm.o  model/Channel/param.o model/EM/main.o model/EM/param.o model/Matter/main.o model/Matter/param.o model/Sensor/main.o model/Sensor/param.o 
}


function build_app {
	cd app  && \
	gcc $1 $3 -c serial_server.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c channels.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread && \
	cd ../   && \
	ar -crvs libpac.a app/channels.o app/serial_server.o app/main.o
}

#1				2
#debug_mode		bin_name
function build {
	#find . -maxdepth 16 -name '*.o' -type f -delete
	build_lib $1 $2 $3 && \
	build_acp $1 $2 $3 && \
	build_acp_serial_server $1 $2 $3 && \
	build_sqlite $1 $2 $3 && \
	build_model $1 $2 $3 && \
	build_app $1 $2 $3 && \
	gcc -D_REENTRANT -DSQLITE_THREADSAFE=2 -DSQLITE_OMIT_LOAD_EXTENSION $1 $3 main.c -o $2 $DEBUG_PARAM -L./ -lpac -lm -lpthread && \
	echo "Application" $2 "has been successfully compiled."
}

function part_debug {
	clear
	clear
	build $MODE_DEBUG $APP_DBG $NONE
}

function full {
	clear
	clear
	DEBUG_PARAM=$NONE
	build $NONE $APP $MODE_FULL && \
	build $MODE_DEBUG $APP_DBG $MODE_FULL && \
	move_bin && move_bin_dbg && move_conf && conf_autostart
}

function full_nc {
	DEBUG_PARAM=$NONE
	build $NONE $APP $MODE_FULL && \
	build $MODE_DEBUG $APP_DBG $MODE_FULL  && \
	move_bin && move_bin_dbg
}


function part {
	build $NONE $APP_DBG $NONE
}

function uninstall_nc {
	pkill $APP --signal 9
	pkill $APP_DBG --signal 9
	rm -f $INST_DIR/$APP
	rm -f $INST_DIR/$APP_DBG
}
function uninstall {
	uninstall_nc
	update-rc.d -f $APP remove
	rm -rf $CONF_DIR_APP
}


f=$1
${f}
