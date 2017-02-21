#! /bin/sh
WII_PATH=$PWD/inc/wiiuse/build/src
if [ -n "$TEST_BUILD_EXEC" ]
then
	cd EMMA/EMMA/release # emma.ini must be in the directory
	LD_LIBRARY_PATH=$WII_PATH ./EMMA
	RET=$?
	if [ $RET -eq 134 ]
	then echo "ci_test.sh: EMMA returned with 134 as expected"
	else exit $RET
	fi
else
	cd EMMA/test
	qmake
	make
	LD_LIBRARY_PATH=$WII_PATH release/test
	exit $?
fi
