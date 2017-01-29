#! /bin/bash
LD_LIBRARY_PATH=inc/wiiuse/build/src EMMA/EMMA/release/EMMA
RET=$?
if [ $RET -eq 134 ]
then echo "ci_test.sh: EMMA returned with 134 as expected"
else exit $RET
fi
