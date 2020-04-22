FROM ubuntu:18.04

ENV OUT_DIR=/app/tmp \
	SNAPSCALE_DIR=/app/snapscale
ENV OUT_BIN=${OUT_DIR}/bin \
	OUT_LIB=${OUT_DIR}/lin

RUN mkdir -p ${SNAPSCALE_DIR}

COPY . ${SNAPSCALE_DIR}

RUN apt-get update -y && \
	apt-get install -y git
	cd ${SNAPSCALE_DIR} && \
	git submodule update --init --recursive

RUN cd ${SNAPSCALE_DIR}/scripts && \
	rm -rf ${OUT_DIR} && \
	./eosio_build.sh -m -y -i ${OUT_DIR} && \
	./eosio_install.sh


FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates libcurl4-openssl-dev libusb-1.0-0-dev && rm -rf /var/lib/apt/lists/*

COPY --from=0 /app/tmp/lib /opt/snapscale/lib
COPY --from=0 /app/tmp/bin /opt/snapscale/bin
COPY --from=0 /app/snapscale/docker/config.ini /opt/snapscale/conf/config.ini
COPY --from=0 /app/snapscale/docker/boot.sh /opt/snapscale/bin/boot.sh

ENV LD_LIBRARY_PATH=/opt/snapscale/lib
ENV PATH=/opt/snapscale/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
