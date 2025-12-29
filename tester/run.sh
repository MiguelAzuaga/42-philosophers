#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../" && pwd)"

BIN="$ROOT_DIR/philo/philo"
CASES_DIR="$SCRIPT_DIR/cases"
OUTFILE="$SCRIPT_DIR/.philo_test.log"

TIMEOUT=5
TIMEOUT_SET=0
KO_LOG=""
KO_LOG_SET=0
PRINT_KO_LOG=0
PRINT_LOG=0

PASS=0
FAIL=0


C_RESET='\033[0m'
C_GREEN='\033[92m'
C_RED='\033[91m'

# Clear all .log files in the current directory
find "$SCRIPT_DIR" -name "*.log" -exec rm -f {} \;

while getopts "t:klL" flag; do
	case $flag in
		t)
			if [[ "$OPTARG" =~ ^[0-9]+$ ]]; then
				TIMEOUT=$OPTARG
				TIMEOUT_SET=1
			else
				echo "Error: -t flag requires an integer value for the timeout."
				exit 1
			fi
			;;
		k) KO_LOG_SET=1 ;;
		l) PRINT_KO_LOG=1 ;;
		L) PRINT_LOG=1 ;;
		*)
			echo "Usage: $0 [-t timeout] [-k] [-l/L] [test_files]"
			exit 1
			;;
	esac
done
shift $((OPTIND - 1))

ok()
{
	echo -e "${C_GREEN}[OK]${C_RESET} $1"
	((PASS++))

	if [[ PRINT_LOG -eq 1 ]]; then
		local file_log="$SCRIPT_DIR/.$1.log"
		cat "$OUTFILE" > "$file_log"
	fi
}

ko()
{
	echo -e "${C_RED}[KO]${C_RESET} $1";
	((FAIL++));
	local file_log="$SCRIPT_DIR/.$1.log"

	if [[ KO_LOG_SET -eq 1 ]]; then
		echo -e "${KO_LOG}\n" > "$file_log"
		echo "log created at $file_log"
	fi

	if [[ PRINT_KO_LOG -eq 1  ]] || [[ PRINT_LOG -eq 1 ]]; then
		cat "$OUTFILE" >> "$file_log"
	fi
}

run_program() {
	if [ -n "$MEALS" ]; then
		$BIN $N $DIE $EAT $SLEEP $MEALS >"$OUTFILE" 2>&1 &
	else
		$BIN $N $DIE $EAT $SLEEP >"$OUTFILE" 2>&1 &
	fi
	PID=$!

	if { [ "$TIMEOUT" -gt 0 ]; } && { [ -z "$MEALS" ] || [ "$TIMEOUT_SET" -eq 1 ]; }; then
		(sleep "$TIMEOUT" && kill -0 $PID 2>/dev/null && kill $PID) &
		TIMEOUT_PID=$!

		wait $PID
		EXIT_STATUS=$?

		kill $TIMEOUT_PID 2>/dev/null

		return $EXIT_STATUS
	else
		wait $PID
		return $?
	fi
}


check_timing()
{
	if [[ -n $MEALS ]]; then
		LAST_LINE=$(tail -n 1 "$OUTFILE")
		[[ ${LAST_LINE,,} =~ eat|die|dead ]] || { KO_LOG="${KO_LOG}Timing issue: Last event was $LAST_LINE (expected philosopher to eat or die)\n"; return 1; }
	fi

	declare -A last_time last_action

	while read -r time id _; do
		line=$REPLY
		case $line in
			*eat*)   action=eat ;;
			*sleep*) action=sleep ;;
			*think*) action=think ;;
			*die*|*dead*) action=die ;;
			*) continue ;;
		esac

		if [[ -z ${last_time[$id]} ]]; then
			last_time[$id]=$time
			last_action[$id]=$action
			continue
		fi

		duration=$(( time - last_time[$id] ))
		[[ ${last_action[$id]} == eat && duration < EAT ]] && { KO_LOG="${KO_LOG}Timing issue: Philosopher $id didn't eat long enough ($duration < $EAT)\n"; return 1; }
		[[ ${last_action[$id]} == sleep && duration < SLEEP ]] && { KO_LOG="${KO_LOG}Timing issue: Philosopher $id didn't sleep long enough ($duration < $SLEEP)\n"; return 1; }

		last_time[$id]=$time
		last_action[$id]=$action
	done < "$OUTFILE"

	return 0
}

check_death()
{
	grep -qiE "die|dead" "$OUTFILE"
	[[ "$?" -eq "$EXPECT_DIE" ]] && { KO_LOG="${KO_LOG}Death issue: Philosopher died unexpectedly or didn't die when expected.\n"; return 1; }
	return 0
}

check_meals()
{
	[ -n "$MEALS" ] || return 0
	for ((i = 1; i <= N; i++)); do
		COUNT=$(grep -cE " $i .*eat" "$OUTFILE")
		if (( COUNT < MEALS )); then
			KO_LOG="${KO_LOG}Meal issue: Philosopher $i ate $COUNT meals, expected at least $MEALS.\n"
			return 1
		fi
	done
	return 0
}

run_test_case()
{
	N=$1; DIE=$2; EAT=$3; SLEEP=$4; MEALS=$5; EXPECT_DIE=$6 KO_LOG=""

	run_program
	check_timing || return 1
	check_death  || return 1
	check_meals  || return 1
	return 0
}

if [ "$#" -eq 0 ]; then
	FILES=("$CASES_DIR"/*.txt)
else
	FILES=()
	for arg in "$@"; do
		FILES+=("$arg")
	done
fi

for file in "${FILES[@]}"; do
	echo "== $(basename "$file") =="

	while read -r name N DIE EAT SLEEP MEALS EXPECT_DIE; do
		[[ "$name" =~ ^#|^$ ]] && continue
		[ "$MEALS" = "-" ] && MEALS=""

		if run_test_case "$N" "$DIE" "$EAT" "$SLEEP" "$MEALS" "$EXPECT_DIE"; then
			ok "$name"
		else
			ko "$name"
		fi
	done < "$file"
done

rm -f "$OUTFILE"

echo
echo "Passed: $PASS"
echo "Failed: $FAIL"
[ "$FAIL" -eq 0 ]
