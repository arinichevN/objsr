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
	cd lib
	gcc $1 $3 -c app.c -D_REENTRANT $DEBUG_PARAM -lpthread && \
	gcc $1 $3 -c util.c -D_REENTRANT $DEBUG_PARAM  && \
	gcc $1 $3 -c timef.c -D_REENTRANT $DEBUG_PARAM  && \
	gcc $1 $3 -c serial.c -D_REENTRANT $DEBUG_PARAM  && \
	gcc $1 $3 -c dbl.c -D_REENTRANT $DEBUG_PARAM  && \
	cd acp && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM  && \
	cd serial && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM  && \
	cd server && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM  && \
	cd PortController && \
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread  && \
	cd Param
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM  && \
	cd ../../../../../ && \

	#echo "library: making archive..." && \
	#rm -f libpac.a
	ar -crv libpac.a app.o util.o timef.o serial.o dbl.o acp/main.o  acp/serial/main.o acp/serial/server/main.o acp/serial/server/PortController/main.o acp/serial/server/PortController/Param/main.o
	cd ../ 
}

function build_sqlite {
	if [ ! -f sqlite3.o ]; then
    gcc $1 $3 -DSQLITE_THREADSAFE=2 -DSQLITE_OMIT_LOAD_EXTENSION  -c sqlite3.c -D_REENTRANT $DEBUG_PARAM -lpthread
    fi
}

function build_model {
	local da=(Channel ChannelParam EM EMParam Matter MatterParam Sensor SensorParam)
	cd model
	
	for d in "${da[@]}"
	do
		#printf "_________________${d}\n" 
		cd ${d} 
		gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM -lpthread
		cd ../
	done
	
	for d in "${da[@]}"
	do
		ar -crv libmodel.a ${d}/main.o 
	done
	
	cd Channel
	gcc $1 $3 -c app.c -D_REENTRANT $DEBUG_PARAM -lpthread
	gcc $1 $3 -c list.c -D_REENTRANT $DEBUG_PARAM -lpthread
	cd ../
	ar -crv libmodel.a Channel/app.o Channel/list.o 
	cd ../ 
}

function build_app {
	cd app
	gcc $1 $3 -c main.c -D_REENTRANT $DEBUG_PARAM  && \
	gcc $1 $3 -c serial_server.c -D_REENTRANT $DEBUG_PARAM  && \
	#rm -f libapp.a
	ar -crv libapp.a main.o serial_server.o
	cd ../ 
}

#1				2
#debug_mode		bin_name
function build {
	#find . -maxdepth 16 -name '*.o' -type f -delete
	build_lib $1 $2 $3 && \
	build_sqlite $1 $2 $3 && \
	build_model $1 $2 $3 && \
	build_app $1 $2 $3 && \
	#gcc -D_REENTRANT -DSQLITE_THREADSAFE=2 -DSQLITE_OMIT_LOAD_EXTENSION $1 $3  main.c -o $2 $DEBUG_PARAM -lpthread -L./lib -L./model -L./app  -lpac -lmodel -lapp
	gcc -D_REENTRANT -DSQLITE_THREADSAFE=2 -DSQLITE_OMIT_LOAD_EXTENSION $1 $3  main.c -o $2 $DEBUG_PARAM  \
	sqlite3.o \
	app/main.o app/serial_server.o model/EM/main.o model/EMParam/main.o model/Matter/main.o model/MatterParam/main.o model/Sensor/main.o model/SensorParam/main.o  \
	model/Channel/main.o model/Channel/list.o model/Channel/app.o model/ChannelParam/main.o \
	-L./lib -lpthread -lm -lpac&& \
	echo "Application successfully compiled. Launch command: ./"$2
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
