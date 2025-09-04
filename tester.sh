#!/bin/bash

# Usage:
#	./test_philo.sh <philo_binary> <nbr> <die> <eat> <sleep> [meals]

# Flags
SILENT=0
TIMEOUT=5
TIMEOUT_SET=0

while getopts "t:n" flag; do
	case $flag in
		n) SILENT=1 ;;
		t) TIMEOUT=$OPTARG; TIMEOUT_SET=1 ;;
		*) echo "Usage: $0 [-t seconds] [-n] <philo_binary> <nbr> <die> <eat> <sleep> [meals]"; exit 1 ;;
	esac
done
shift $((OPTIND - 1))

# Check remaining arguments
if [ "$#" -lt 5 ] || [ "$#" -gt 6 ]; then
	echo "Usage: $0 [-t seconds] [-n] <philo_binary> <nbr> <die> <eat> <sleep> [meals]"
	exit 1
fi

# Colors & styles
ESC='\033['
RST='0m'
B='1;'
C_RED='91m'
C_GREEN='92m'
C_YELLOW='93m'
C_BLUE='94m'
C_MAGENTA='95m'

# Vars
BIN=$1
N=$2
DIE=$3
EAT=$4
SLEEP=$5
MEALS=$6
VALID=1
OUTFILE="philo_test.log"

# Ensure numeric values
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
echo -e "${ESC}${C_MAGENTA}▶ Argument Validation${ESC}${RST}"
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"

for varname in N DIE EAT SLEEP MEALS; do
	val="${!varname}"
	# Skip empty MEALS (optional argument)
	if [ "$varname" = "MEALS" ] && [ -z "$val" ]; then
		continue
	fi
	if ! [[ "$val" =~ ^[0-9]+$ ]]; then
		echo -e "${ESC}${C_RED}✖${ESC}${RST} Error: $varname must be a positive integer (got '$val')" >&2
		VALID=0
	else
		echo -e "${ESC}${C_GREEN}✔${ESC}${RST} $varname	= $val"
	fi
done
if [ $VALID -eq 1 ]; then
	echo -e "${ESC}${C_GREEN}✔${ESC}${RST} All required arguments are valid"
else
	echo -e "${ESC}${C_RED}✖${ESC}${RST} Some arguments are invalid"
fi

# Run binary in the background
echo -e "${ESC}${B}${C_YELLOW}Running:${ESC}${RST} $BIN $N $DIE $EAT $SLEEP ${MEALS:-}"
if [ -n "$MEALS" ]; then
	$BIN $N $DIE $EAT $SLEEP $MEALS > $OUTFILE 2>&1 &
else
	$BIN $N $DIE $EAT $SLEEP > $OUTFILE 2>&1 &
fi
PID=$!

# Check for program errors
if grep -qiE "error|Invalid|usage" "$OUTFILE"; then
	echo -ne "${ESC}${C_RED}✖${ESC}${RST} Program reported an error: "
	grep -iE "error|Invalid|usage" "$OUTFILE"
	echo -e "${ESC}${C_RED}✖${ESC}${RST} Skipping further checks because program did not run correctly."
	rm -f "$OUTFILE"
	exit 1
fi

# Enforce TIMEOUT if no MEALS specified OR if -t flag was used
if { [ -z "$MEALS" ] && [ "$TIMEOUT" -gt 0 ]; } || { [ -n "$MEALS" ] && [ "$TIMEOUT_SET" -eq 1 ]; }; then
	(
		sleep "$TIMEOUT"
		if kill -0 $PID 2>/dev/null; then
			echo -e "${ESC}${B}${C_YELLOW}Stopping program after${ESC}${RST} $TIMEOUT seconds..."
			kill $PID
			sleep 0.1
			kill -0 $PID 2>/dev/null && kill -9 $PID
		fi
	) &
	WATCHDOG=$!
fi

# Wait for the binary to finish
wait $PID
[ -n "$WATCHDOG" ] && kill $WATCHDOG 2>/dev/null

# Output program log
if [ "$SILENT" == 0 ]; then
	echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
	echo -e "${ESC}${C_MAGENTA}▶ Program Output${ESC}${RST}"
	echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
	cat $OUTFILE
fi

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
if grep -qiE "die|dead" $OUTFILE; then
	echo -e "${ESC}${C_RED}✖${ESC}${RST} A philosopher died!"
else
	echo -e "${ESC}${C_GREEN}✔${ESC}${RST} No philosopher died"
fi

# Timing check
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
echo -e "${ESC}${C_MAGENTA}▶ Timing Check${ESC}${RST}"
echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"

LAST_LINE=$(tail -n 1 "$OUTFILE")
if echo "$LAST_LINE" | grep -Eq "eat|die|dead"; then
	echo -e "${ESC}${C_GREEN}✔${ESC}${RST} Last event is valid"
else
	echo -e "${ESC}${C_RED}✖${ESC}${RST} Last event was: $LAST_LINE (expected philosopher to eat or die)"
fi

awk -v eat="$EAT" -v sleep="$SLEEP" -v die="$DIE" -v last_action=$LAST_LINE'
{
	time=$1; id=$2; line=$0

	action = ""
	if (line ~ /eat/)			action = "eat"
	else if (line ~ /sleep/)	action = "sleep"
	else if (line ~ /think/)	action = "think"
	else if (line ~ /die|dead/)	action = "die"

    # initialize philosopher state
    if (!(id in last_time))
	{
        last_time[id] = time
        last_eat[id] = time
        last_action[id] = action
        next
    }

	# starvation check BEFORE updating
    if (time - last_eat[id] > die && action != "die")
	{
        printf "\033[91m✖\033[0m Philosopher %d should have died before %s at %d (last ate %d, die=%d)\n",
			id, action, time, last_eat[id], die
        bad=1
    }

	duration = time - last_time[id]

	if (last_action[id] == "eat" && duration < eat)
	{
		printf "\033[91m✖\033[0m Philosopher %d did not eat long enough (%d < %d)\n"
			, id, duration, eat
		bad=1
	}
	if (last_action[id] == "sleep" && duration < sleep)
	{
		printf "\033[91m✖\033[0m Philosopher %d did not sleep long enough (%d < %d)\n",
			id, duration, sleep
		bad=1
	}

	# update tracking
    last_time[id] = time
    last_action[id] = action
    if (action == "eat")
	{
        last_eat[id] = time
    }
}
END {
    split(last_line, parts, " ")
    end_time = parts[1]

    for (id in last_eat)
	{
        if (end_time - last_eat[id] > die && last_action[id] != "die") {
            printf "\033[91m✖\033[0m Philosopher %d should have died by the end (last ate %d, end %d, die=%d)\n",
			id, last_eat[id], end_time, die
            bad=1
        }
    }
	if (!bad)
	{
		printf "\033[92m✔\033[0m All actions respected their timing\n"
	}
}
' "$OUTFILE"

# Cleanup
rm -f "$OUTFILE"
