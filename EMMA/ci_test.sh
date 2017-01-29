#! /bin/bash
LD_LIBRARY_PATH=inc/wiiuse/build/src EMMA/EMMA/release/EMMA
if [ $? -eq 134 ]
then echo "ci_test.sh: EMMA returned with 134 as expected"
else return $?
fi
