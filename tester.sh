#!/bin/bash
#------------------------------------------------------------
# Philosophers Tester
# Usage:
#   ./test_philo.sh [-t seconds] [-n] [-a] <philo_binary> <nbr> <die> <eat> <sleep> [meals]
#------------------------------------------------------------

#------------------------------------------------------------
# Config / Defaults
#------------------------------------------------------------
SILENT=0
TIMEOUT=5
TIMEOUT_SET=0
RUN_ALL=0
OUTFILE="philo_test.log"

# Colors
ESC='\033['
RST='0m'
B='1;'
C_RED='91m'
C_GREEN='92m'
C_YELLOW='93m'
C_BLUE='94m'
C_MAGENTA='95m'

PASS_COUNT=0
FAIL_COUNT=0

#------------------------------------------------------------
# Functions
#------------------------------------------------------------

usage()
{
	echo "Usage: $0 [-t seconds] [-n] [-a] <philo_binary> <nbr> <die> <eat> <sleep> [meals]"
	echo "    -t <s>  set timeout in seconds"
	echo "    -n	  silent mode (no program output)"
	echo "    -a	  run all predefined scenarios"
	exit 1
}

header()
{
	echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
	echo -e "${ESC}${C_MAGENTA}▶ $1${ESC}${RST}"
	echo -e "${ESC}${C_BLUE}------------------------------------------------------------${ESC}${RST}"
}

validate_args()
{
	header "Argument Validation"
	VALID=1
	for varname in N DIE EAT SLEEP MEALS; do
		val="${!varname}"
		[ "$varname" = "MEALS" ] && [ -z "$val" ] && continue
		if ! [[ "$val" =~ ^[0-9]+$ ]]; then
			echo -e "${ESC}${C_RED}✖${ESC}${RST} Error: $varname must be a positive integer (got '$val')"
			VALID=0
		else
			echo -e "${ESC}${C_GREEN}✔${ESC}${RST} $varname = $val"
		fi
	done
	[ $VALID -eq 0 ] && usage
}

run_program()
{
	header "Run Program"
	echo -e "${ESC}${B}${C_YELLOW}Running:${ESC}${RST} $BIN $N $DIE $EAT $SLEEP ${MEALS:-}"
	if [ -n "$MEALS" ]; then
		$BIN $N $DIE $EAT $SLEEP $MEALS >"$OUTFILE" 2>&1 &
	else
		$BIN $N $DIE $EAT $SLEEP >"$OUTFILE" 2>&1 &
	fi
	PID=$!

	# Watchdog (timeout)
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

	wait $PID
	[ -n "$WATCHDOG" ] && kill $WATCHDOG 2>/dev/null
}

show_output()
{
	[ "$SILENT" -eq 1 ] && return
	header "Program Output"
	cat "$OUTFILE"
}

check_meals()
{
	header "Meal Count"
	local failed=0
	local philos_less=""

	for ((i = 1; i <= N; i++)); do
		COUNT=$(grep -cE " $i .*eat" "$OUTFILE")
		if [ -n "$MEALS" ]; then
			if [ "$COUNT" -lt "$MEALS" ]; then
				[ "$SILENT" -eq 0 ] && echo -e "${ESC}${C_RED}✖${ESC}${RST} Philosopher $i ate too few meals (expected $MEALS, got $COUNT)"
				failed=1
				philos_less="$philos_less $i"
			else
				[ "$SILENT" -eq 0 ] && echo -e "${ESC}${C_GREEN}✔${ESC}${RST} Philosopher $i ate $COUNT meals (expected $MEALS)"
			fi
		else
			if [ "$COUNT" -eq 0 ]; then
				[ "$SILENT" -eq 0 ] && echo -e "${ESC}${C_RED}✖${ESC}${RST} Philosopher $i never ate!"
				failed=1
				philos_less="$philos_less $i"
			else
				[ "$SILENT" -eq 0 ] && echo "Philosopher $i ate $COUNT meals"
			fi
		fi
	done

	# Silent summary
	if [ "$SILENT" -eq 1 ] && [ -n "$MEALS" ]; then
		if [ -z "$philos_less" ]; then
			echo -e "${ESC}${C_GREEN}✔${ESC}${RST} All philosophers ate the expected amount of meals"
		else
			echo -e "${ESC}${C_RED}✖${ESC}${RST} Philosophers${philos_less} ate less than $MEALS meals"
		fi
	fi

	return $failed
}

check_death()
{
	if grep -qiE "die|dead" "$OUTFILE"; then
		echo -e "${ESC}${C_RED}✖${ESC}${RST} A philosopher died!"
		return 1
	else
		echo -e "${ESC}${C_GREEN}✔${ESC}${RST} No philosopher died"
		return 0
	fi
}

check_timing()
{
	header "Timing Check"
	if [ -n "$MEALS" ]; then
		LAST_LINE=$(tail -n 1 "$OUTFILE")
		if echo "$LAST_LINE" | grep -Eq "eat|die|dead"; then
			echo -e "${ESC}${C_GREEN}✔${ESC}${RST} Last event is valid"
		else
			echo -e "${ESC}${C_RED}✖${ESC}${RST} Last event was: $LAST_LINE (expected philosopher to eat or die)"
			return 1
		fi
	fi

	awk -v eat="$EAT" -v sleep="$SLEEP" '
	{
		time=$1; id=$2; line=$0
		action=""
		if (line ~ /eat/)	   	action="eat"
		else if (line ~ /sleep/)	action="sleep"
		else if (line ~ /think/)	action="think"
		else if (line ~ /die|dead/)	action="die"

		if (last_time[id] == "" || last_action[id] == "") {
			last_time[id] = time
			last_action[id] = action
			next
		}
		duration = time - last_time[id]

		if (last_action[id] == "eat" && duration < eat) {
			printf "\033[91m✖\033[0m Philosopher %d did not eat long enough (%d < %d)\n", id, duration, eat
			bad=1
		}
		if (last_action[id] == "sleep" && duration < sleep) {
			printf "\033[91m✖\033[0m Philosopher %d did not sleep long enough (%d < %d)\n", id, duration, sleep
			bad=1
		}
		last_time[id] = time
		last_action[id] = action
	}
	END {
		if (!bad) {
			printf "\033[92m✔\033[0m All actions respected their timing\n"
		}
	}' "$OUTFILE"
}

cleanup()
{
	rm -f "$OUTFILE"
}

run_test()
{
	validate_args
	run_program
	show_output
	check_meals
	local m=$?
	check_death
	local d=$?
	check_timing
	local t=$?
	cleanup

	while [ "$#" -gt 0 ]; do
		case "$1" in
			m) m=$((1 - m)) ;;
			d) d=$((1 - d)) ; m=0;;
			t) t=$((1 - t)) ;;
			*) echo "Unknown flag '$1'";;
		esac
		shift
	done

	if [ $m -eq 0 ] && [ $d -eq 0 ] && [ $t -eq 0 ]; then
		PASS_COUNT=$((PASS_COUNT + 1))
		echo -e "${ESC}${B}${C_GREEN}✔ Test Passed${ESC}${RST}"
	else
		FAIL_COUNT=$((FAIL_COUNT + 1))
		echo -e "${ESC}${B}${C_RED}✖ Test Failed${ESC}${RST}"
	fi
	echo
}

run_all()
{
	BIN=$1
	shift 1

	echo -e "${ESC}${B}${C_YELLOW}Running every test...${ESC}${RST}"
	echo

	# To create a test, insert each argument as follows:
	# To run the test use run_test with flag d if philo is expected to die
	#N=;	DIE=;	EAT=;	SLEEP=;	MEALS=;
	#run_test [d]

	# 1 philosopher (should die)
	N=1; DIE=800; EAT=200; SLEEP=200; MEALS=""
	run_test d

	# 2 philosophers (should alternate, no deaths)
	N=2; DIE=800; EAT=200; SLEEP=200; MEALS=""
	run_test

	# 5 philosophers (stress)
	N=5; DIE=800; EAT=200; SLEEP=200; MEALS=""
	run_test

	# 5 philosophers with meals (must exit cleanly)
	N=5; DIE=800; EAT=200; SLEEP=200; MEALS=7
	run_test

	# Many philosophers (stress)
	N=50; DIE=800; EAT=200; SLEEP=200; MEALS=3
	run_test

	# Extra arguments passed as last scenario (optional)
	if [ "$#" -ge 4 ]; then
		N=$1; DIE=$2; EAT=$3; SLEEP=$4; MEALS=$5
		shift 4
		[ -z "$MEALS" ] || shift 1
		run_test "$@"
	fi

	header "Summary: ${ESC}${C_GREEN}$PASS_COUNT passed${ESC}${RST}, ${ESC}${C_RED}$FAIL_COUNT failed${ESC}${RST}"
}

#------------------------------------------------------------
# Main
#------------------------------------------------------------

while getopts "t:na" flag; do
	case $flag in
	n) SILENT=1 ;;
	a) RUN_ALL=1 ;;
	t) TIMEOUT=$OPTARG; TIMEOUT_SET=1 ;;
	*) usage ;;
	esac
done
shift $((OPTIND - 1))

if [ "$RUN_ALL" -eq 1 ]; then
	[ "$#" -lt 1 ] && usage
	run_all "$@"
	exit 0
fi

# Single test mode
[ "$#" -lt 5 ] || [ "$#" -gt 7 ] && usage
BIN=$1; N=$2; DIE=$3; EAT=$4; SLEEP=$5; MEALS=$6
shift 5
[ -z "$MEALS" ] || shift 1
run_test "$@"
