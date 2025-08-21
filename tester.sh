#!/bin/bash

# Usage:
#   ./test_philo.sh <philo_binary> <nbr> <die> <eat> <sleep> [meals]

if [ "$#" -lt 5 ] || [ "$#" -gt 6 ]; then
	echo "Usage: $0 <philo_binary> <nbr> <die> <eat> <sleep> [meals]"
	exit 1
fi

TIMEOUT=5

ESC='\033['
RST='0m'

# Styles
B='1;'
D='2;'
I='3;'
U='4;'
BL='5;'
ST='9;'

# Colors
C_RED='91m'
C_GREEN='92m'
C_YELLOW='93m'
C_BLUE='94m'
C_MAGENTA='95m'

BIN=$1
N=$2
DIE=$3
EAT=$4
SLEEP=$5
MEALS=$6
OUTFILE="philo_test.log"

echo -e "${ESC}${B}${C_YELLOW}Running:${ESC}${RST} $BIN $N $DIE $EAT $SLEEP $MEALS"
if [ -n "$6" ]; then
	$BIN $N $DIE $EAT $SLEEP $MEALS > $OUTFILE 2>&1 &
else
	$BIN $N $DIE $EAT $SLEEP > $OUTFILE 2>&1 &
fi
PID=$!

# Run up to TIMEOUT seconds
sleep $TIMEOUT
if kill -0 $PID 2>/dev/null; then
	echo -e "${ESC}${B}${C_YELLOW}Stopping program after${ESC}${RST} $TIMEOUT seconds..."
	kill $PID
	sleep 0.1
	kill -0 $PID 2>/dev/null && kill -9 $PID
fi
wait $PID 2>/dev/null

echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
echo -e "${ESC}${C_MAGENTA}▶ Program Output${ESC}${RST}"
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
cat $OUTFILE

# Count meals per philosopher
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
echo -e "${ESC}${C_MAGENTA}▶ Meal Count${ESC}${RST}"
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
for ((i=1; i<=N; i++)); do
	COUNT=$(grep -cE " $i .*eat" $OUTFILE)
	if [ -n "$MEALS" ]; then
		if [ "$COUNT" -lt "$MEALS" ]; then
			echo -e "${ESC}${C_RED}✖${ESC}${RST} Philosopher $i ate too few meals (expected $MEALS, got $COUNT)"
		else
			echo -e "${ESC}${C_GREEN}✔${ESC}${RST} Philosopher $i ate $COUNT meals (expected $MEALS)"
		fi
	else
		echo -e "Philosopher $i ate $COUNT meals"
	fi
done

# Check if someone died
if grep -q "died" $OUTFILE; then
	echo -e "${ESC}${C_RED}✖${ESC}${RST} A philosopher died!"
else
	echo -e "${ESC}${C_GREEN}✔${ESC}${RST} No philosopher died"
fi
rm -f "$OUTFILE"
