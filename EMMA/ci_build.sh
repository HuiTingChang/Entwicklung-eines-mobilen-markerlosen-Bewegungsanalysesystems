#! /bin/bash
# submodule is now updated through gitlab-runner
#git submodule init
#git submodule update --depth 1
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
