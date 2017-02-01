#! /bin/bash
cd EMMA/EMMA/release # emma.ini must be in the directory
LD_LIBRARY_PATH=$OLDPWD/inc/wiiuse/build/src ./EMMA
RET=$?
if [ $RET -eq 134 ]
then echo "ci_test.sh: EMMA returned with 134 as expected"
else exit $RET
fi
