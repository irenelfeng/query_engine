#!/bin/bash
#
# Description: will test query and output results into a log file.
#

# (1) Create a log file to save all output of the test script

log="queryTestlog.$(date +%a_%b_%d_%Y)"
readable_date="$(date +%Y-%m-%d) $(date +%H:%M)"

echo "Starting query tests..."
printf "Test log on query for $readable_date\n" > $log
printf "Build details: \n host: $HOSTNAME \n" >> $log
printf " operating system: " >> $log
uname >> $log

# (2) build and test the query engine
printf "\nCalling make clean and make query:\n " >> $log
make clean >> $log 2>&1
make query >> "$log" 2>&1
if [ $? -ne 0 ]; then
	printf "Build Failed." >> $log 
	exit 1
fi

printf "\nCalling make query_test:\n " >> $log
make query_test  >> $log 2>&1
if [ $? -ne 0 ]; then
	printf "Build query_test Failed." >> $log 
	exit 1
fi

printf "\nCalling query_test for unit tests:\n " >> $log
cd test
./query_test >> ../$log 2>&1 #need to go down a folder t ogo to right log file
if [ $? -eq 1 ]; then
    echo "Failed unit test" >> ../$log
    exit 1
fi
cd ..

# Do some invalid input testing
printf "Test Invalid Number of Inputs \n\t./query \n" >> $log
./query >> $log 2>&1 
if [ $? -ne 1 ]; then
	printf "Warning: An input that should give failure does not result in a failure code." >> $log
fi
printf "\n" >> $log

printf "Test Invalid index file \n\t./query invalid_index_file ../ \n" >> $log
./query invalid_index_file ../ >> $log 2>&1 
if [ $? -ne 1 ]; then
	printf "Warning: An input that should give failure does not result in a failure code." >> $log
fi
printf "\n" >> $log

printf "Not a directory \n\t./query invalid_index_file not_directory \n" >> $log
./query test/test_index not_directory >> $log 2>&1 
if [ $? -ne 1 ]; then
	printf "Warning: An input that should give failure does not result in a failure code." >> $log
fi
printf "\n" >> $log

printf "\n PASSED TESTING. query engine works!"  >> "$log"
echo "Testing finished. see $log for details."
