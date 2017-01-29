FROM gcc

LABEL maintainer "YWashio@EONERC.RWTH-Aachen.de"

RUN apt-get update
RUN apt-get install -y \
	qt5-default \
	git
RUN apt-get install -y \
	libopencv-dev
RUN apt-get install -y \
	cmake \
	libbluetooth-dev
RUN rm -r /var/lib/apt/lists/*
