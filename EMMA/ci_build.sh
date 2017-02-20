#! /bin/sh
# submodule is currently not updated through gitlab-runner
# because of some certificate problem
FIRSTPWD=$PWD
git submodule init
git submodule update --depth 1
cd inc/wiiuse && \
git checkout master && \
mkdir -p build && \
cd build && \
cmake .. && \
make wiiuse
cd $FIRSTPWD && \
cd EMMA/EMMA && \
qmake && \
make && \
make install
