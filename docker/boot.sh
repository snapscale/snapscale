#!/bin/sh

DATA_DIR=/opt/snapscale/data

if [ ! -d "${DATA_DIR}" ]; then
    mkdir ${DATA_DIR} 
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_FILE=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_FILE" ]; then
    CONFIG_FILE="--config=/opt/snapscale/conf/config.ini"
else
    CONFIG_FILE=""
fi

exec /opt/snapscale/bin/nodeos --data-dir=${DATA_DIR} --config=$CONFIG_FILE "$@"


