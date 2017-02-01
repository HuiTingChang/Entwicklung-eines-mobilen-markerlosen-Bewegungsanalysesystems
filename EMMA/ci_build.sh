#! /bin/bash
# submodule is currently not updated through gitlab-runner
# because of some certificate problem
git submodule init
git submodule update --depth 1
pushd inc/wiiuse
git checkout master
mkdir -p build
cd build
cmake ..
make wiiuse
popd
cd EMMA/EMMA
qmake
make
make install
