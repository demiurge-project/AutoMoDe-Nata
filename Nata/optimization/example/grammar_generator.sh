#!/bin/bash

# Syntax printing
function print_syntax() {
    echo
    echo "To generate the txt with the descrition of the possible grammar for the finite state machine"
    echo "specify MAX_NBR_STATES, MAX_NBR_CONNECTIONS, and the txt file name in which the description will be saved:"
    echo "$0 <MAX_NBR_STATES> <MAX_NBR_CONNECTIONS> <TXT_FILE> <SIZE_REP>"
    echo
    exit 1
}

# Write description of state
function write_state() {
  INDEX=$1
  NB_TRANS=$2
  REPERTOIRE_CAT="("
  for i in `seq 0 $((SIZE_REP-1))`
    do REPERTOIRE_CAT+=$i","
  done
  REPERTOIRE_CAT+=$SIZE_REP")"
  echo "S$INDEX     \"--s$INDEX \"  c $REPERTOIRE_CAT | as.numeric(NumStates)>$INDEX " >> ${TXT_FILE}
  if [ ${INDEX} == 0 ]; then
    echo "NumConnections$INDEX \"--n$INDEX \" i (1,$NB_TRANS) | as.numeric(NumStates)>1" >> ${TXT_FILE}
  else
    echo "NumConnections$INDEX \"--n$INDEX \" i (1,$NB_TRANS) | as.numeric(NumStates)>${INDEX}" >> ${TXT_FILE}
  fi
}

function write_connection() {
  STATE=$1
  CONNECTION=$2
  create_range STATE
  echo "N${STATE}x$CONNECTION  \"--n${STATE}x$CONNECTION \" i   (0,3) | as.numeric(NumConnections$STATE)>$CONNECTION " >> ${TXT_FILE}
  echo "C${STATE}x$CONNECTION  \"--c${STATE}x$CONNECTION \" c   (0,1,2,3,4,5) | as.numeric(NumConnections$STATE)>$CONNECTION " >> ${TXT_FILE}
  echo "P${STATE}x$CONNECTION  \"--p${STATE}x$CONNECTION \" r   (0,1) | as.numeric(C${STATE}x$CONNECTION) %in% c(0,1,2,5) " >> ${TXT_FILE}
  echo "B${STATE}x$CONNECTION  \"--p${STATE}x$CONNECTION \" i   (1,10) | as.numeric(C${STATE}x$CONNECTION)==3 " >> ${TXT_FILE}
  echo "W${STATE}x$CONNECTION  \"--w${STATE}x$CONNECTION \" r   (0,20) | as.numeric(C${STATE}x$CONNECTION)==3 " >> ${TXT_FILE}
  echo "BI${STATE}x$CONNECTION  \"--p${STATE}x$CONNECTION \" i   (1,10) | as.numeric(C${STATE}x$CONNECTION)==4 " >> ${TXT_FILE}
  echo "WI${STATE}x$CONNECTION  \"--w${STATE}x$CONNECTION \" r   (0,20) | as.numeric(C${STATE}x$CONNECTION)==4 " >> ${TXT_FILE}
}

function create_range() {
  FORBIDDEN=$1
  RANGE="("
  for I in $(seq 0 $MAX_NBR_STATES)
  do
    if [ ${I} != ${STATE} ]; then
      RANGE="${RANGE}${I},"
    fi
  done
  RANGE=${RANGE::-1}
  RANGE="${RANGE})"
}


if [ $# -lt 4 ]; then
    print_syntax
fi

MAX_NBR_STATES=$(echo "$1-1" | bc)
MAX_NBR_CONNECTIONS=$(echo "$2-1" | bc)
TXT_FILE=$3
SIZE_REP=$(echo "$4-1" | bc)

# Clear content of file
truncate -s 0 $TXT_FILE

# Write grammar in file
echo "NumStates   \"--nstates \"   i (1,$1)" >> $TXT_FILE
for STATE in $(seq 0 $MAX_NBR_STATES)
do
  write_state $STATE $2
  for CONNECTION in $(seq 0 $MAX_NBR_CONNECTIONS)
  do
    write_connection $STATE $CONNECTION
  done
done
