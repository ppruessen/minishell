#!/bin/bash

# MINISHELL-TESTER

RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

function timestamp()
{
	date +"%d.%m.%Y: %T" # current time
}

function exec_test()
{
# COMMAND LINE OUTPUT
	printf 'Test: %s' "$*"

# OUT_MINISHELL.TXT
	echo 'Test:' "$*" >> out_minishell.txt
	TEST1=$(echo $@ | ./minishell >> out_minishell.txt 2>&1)
	echo "Exit-Status: $?" >> out_minishell.txt
	echo "---" >> out_minishell.txt

# OUT_BASH.TXT
	echo 'Test:' "$*" >> out_bash.txt
	TEST2=$(echo $@ | bash >> out_bash.txt 2>&1)
	echo "Exit-Status: $?" >> out_bash.txt
	echo "---" >> out_bash.txt

# COMPARE
	TMP1=$(echo $@ | ./minishell 2>&1)
#	printf "TMP1: $TMP1"
#	TEST1=$(echo $@ "" | ../test_repos/FloM73/minishell 2>&1)
#	TEST1=$(echo $@ "" | ../test_repos/Dimi20cen/minishell 2>&1)
	ES_1=$?
	TMP2=$(echo $@ | bash 2>&1)
#	echo
#	printf "TMP2: $TMP2"
#	TMP=$(echo $@ "" | ../test_repos/FloM73/minishell 2>&1)
	ES_2=$?
#	PREPARE OUTPUT
	if [ "$TMP1" != "$TMP2" ]; then
#		TMP1=${TMP1/"_=./minishell"/ ""}
		TMP2=${TMP2/" line 1: "/ ""}
	fi
	if [ "$TMP1" == "$TMP2" ] && [ "$ES_1" == "$ES_2" ]; then
		printf " $BOLDGREEN%s$RESET" "✓ "
	else
		printf " $BOLDRED%s$RESET" "✗ "
	fi
	printf "$CYAN \"$@\" $RESET"
	if [ "$TMP1" != "$TMP2" ] || [ "$ES_1" != "$ES_2" ]; then
		echo
		echo
		printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TMP1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TMP2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		echo
		echo
		printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
		printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
	fi
# Let the script wait for a keypress before executing the next test
#	echo "Press any key to continue"
#	read -n 1 -s

# COMMAND LINE OUTPUT
	echo
	sleep 0.1
}

printf "\n"
printf "$BOLDGREEN __  __ _____ _   _ _____  _____ _    _ ______ _      _      \n"
printf "|  \/  |_   _| \ | |_   _|/ ____| |  | |  ____| |    | |     \n"
printf "| \  / | | | |  \| | | | | (___ | |__| | |__  | |    | |     \n"
printf "| |\/| | | | | . \` | | |  \___ \|  __  |  __| | |    | |     \n"
printf "| |  | |_| |_| |\  |_| |_ ____) | |  | | |____| |____| |_____\n"
printf "|_|  |_|_____|_| \_|_____|_____/|_|  |_|______|______|pamela|\n$RESET"

# PRINT TIMESTAMP
timestamp
echo

# DELETE FILES
rm -f out_minishell.txt out_bash.txt out_diff.txt

## Simple Command & global
exec_test '/bin/ls'
exec_test '   /bin/ls   '
exec_test '		/bin/ls		'
exec_test '/bin/pwd'
exec_test '  /bin/pwd   	'
exec_test '        exit'
exec_test '/sbin/ifconfig'
exec_test ''
exec_test '        '
exec_test '			'
exec_test '  		  '
exec_test '		  		'
exec_test 'cd'
exec_test '      cd    '
## Arguments & history
exec_test '/bin/ls -la'
exec_test '/bin/ls       -la'
exec_test '  /bin/ls      -la  '

#echo
exec_test '        echo     -n'
exec_test 'echo'
exec_test '			echo		'
exec_test '			echo	-nnnn	'
exec_test '			echo	-n-n-n	'
exec_test '			echo	-n -nn -nnn	'

##exit
exec_test '        exit   42'
exec_test 'exit 42'
exec_test 'exit    -42'
exec_test 'exit - 42'

##return value of a process
exec_test '/bin/ls -la'
exec_test 'echo $?'
exec_test '/bin/ls -doesnotexist'
exec_test 'echo $?'
exec_test 'echo $?'
exec_test 'expr $? + $?'

##signals ----> check manually

##Double quotes
exec_test '     "ls -la"    '
exec_test ' echo    "cat lol.c | cat >lol.c"    '
exec_test '     "     pwd     "    '

##Single quotes
exec_test "     ls     '-la'    "
exec_test "     echo     '-n'    "
exec_test "     echo     '   -n'    "
exec_test "     echo     '-n   '    "
exec_test "     echo     ''    "
exec_test "     ls     ''    "
exec_test " echo    '$USER'    "
exec_test "     '   '    "
exec_test " 'ls    -la |     wc   -l'    "

##unset
exec_test '	unset     M '
exec_test '	export     M=21 '
exec_test "	unset     'M' "

##cd
exec_test '	cd  '
exec_test '	cd  notexist'
exec_test '	cd  .. '
exec_test '	cd..  '
exec_test '	cd  cd    '
exec_test '	cd  . '
exec_test "	cd  '.'  "
exec_test "	cd  '..'  "
exec_test '	cd ".."  '
exec_test "	cd  ../../..  "
exec_test "	pwd "

##pwd
exec_test "pwd"
exec_test "	pwd "
exec_test "	cd .. "
exec_test "	pwd "

##relative path
exec_test 'cd /'

#pipes
exec_test 'ls -la | grep mel | wc -l'
exec_test 'ls -la | grep mel | wctert -l'
exec_test 'ls -la | grep "mel"'
exec_test 'ls -la | grep mel'
exec_test "ls -la | grep 'mel'"


# # TESTS FROM OTHER MINISHELLS THAT PASSED
exec_test ''
exec_test '$DONTEXIST'
exec_test '$LESS$VAR'
exec_test './minishell'
exec_test '/bin/echo'
exec_test '/bin/echo Hola Que Tal'
exec_test '< pwd'
exec_test 'cat M*le'
exec_test 'cat Make*file'
exec_test 'cat Makefile | cat -e | cat -e'
exec_test 'cat Makefile | grep srcs | cat -e'
exec_test 'cat Makefile | grep srcs | grep srcs | cat -e'
exec_test 'cat | cat | cat | ls'
exec_test 'cd'
exec_test 'cd $HOME'
exec_test 'cd $HOME $HOME'
exec_test 'cd .'
exec_test 'cd ..'
exec_test 'cd .. | echo "hola"'
exec_test 'cd .. | pwd'
exec_test 'cd ../'
exec_test 'cd ../.'
exec_test 'cd ../..'
exec_test 'cd ../../../../../../..'
exec_test 'cd .././././.'
exec_test 'cd ./'
exec_test 'cd ./././.'
exec_test 'cd ././././'
exec_test 'cd /'
exec_test 'cd / | echo "hola"'
exec_test 'cd /etc'
exec_test 'echo'
exec_test 'echo '
exec_test 'echo "" hola'
exec_test 'echo ""$HOME'
exec_test 'echo "$DONTEXIST" "Makefile"'
exec_test 'echo "$DONTEXIST""Makefile"'
exec_test 'echo "$DONTEXIST"Makefile'
exec_test 'echo "-n" Hola'
exec_test 'echo "ME"'
exec_test 'echo "echo"'
exec_test 'echo "h'o'la"'
exec_test 'echo "hola"'
exec_test 'echo $'
exec_test 'echo $?'
exec_test 'echo $? | echo $? | echo $?'
exec_test 'echo $DONTEXIST Hola'
exec_test 'echo $HOLA'
exec_test 'echo $HOME'
#exec_test 'echo $HOME%'
exec_test 'echo $HOME9'
exec_test 'echo $hola'
exec_test "echo 'echo'"
exec_test 'echo *t'
exec_test 'echo *t hola'
exec_test 'echo --------n'
exec_test 'echo -n'
exec_test 'echo -n -n'
exec_test 'echo -n -n Hola Que'
exec_test 'echo -n -nnn -nnnn'
exec_test 'echo -n -nnn hola -nnnn'
exec_test 'echo -n -nnn-nnnn'
exec_test 'echo -n Hola'
exec_test 'echo -n-nnn -nnnn'
exec_test 'echo -nHola'
exec_test 'echo -nnn --------n'
exec_test 'echo -nnn --------nnnn'
exec_test 'echo -nnn -----nn---nnnn'
exec_test 'echo -nnnn $hola'
exec_test 'echo -nnnnn'
exec_test 'echo -p'
exec_test 'echo Hola'
exec_test 'echo Hola -n'
exec_test 'echo Hola Que Tal'
exec_test 'echo Le path de mon HOME est $HOME'
exec_test 'echo ME'
exec_test 'echo echo'
exec_test 'echo ho''''la'
exec_test 'echo hola'
exec_test 'echo hola | cat'
exec_test 'echo hola | cat | cat | cat | cat | cat | grep hola'
exec_test 'echo hola | echo que tal'
exec_test 'echo hola |cat'
exec_test 'echo hola|cat'
exec_test 'echo my shit terminal is [$TERM4'
exec_test 'echo my shit terminal is [$TERM4]'
exec_test 'echo my shit terminal is [$TERM]'
exec_test 'echo oui | echo non | echo hola | grep hola'
exec_test 'echo oui | echo non | echo hola | grep non'
exec_test 'echo oui | echo non | echo hola | grep oui'
exec_test 'echo | echo'
exec_test 'env | echo hola'
exec_test 'exit'
exec_test 'exit "666"'
exec_test 'exit +0'
exec_test 'exit +42'
exec_test 'exit +666'
exec_test 'exit -9223372036854775808'
exec_test 'exit 0'
exec_test 'exit 000042'
exec_test 'exit 259'
exec_test 'exit 26632'
exec_test 'exit 42'
exec_test 'exit 666'
exec_test 'exit | ls'
#exec_test 'exit | sleep 1'
exec_test 'export HOL=A=""'
#exec_test 'export HOLA=bonjour | cat -e | cat -e'
exec_test 'export hola | unset hola | echo $?'
exec_test 'export | echo hola'
exec_test 'ifconfig | grep ":"'
exec_test 'ifconfig | grep hola'
exec_test 'ls'
exec_test 'ls *.*'
exec_test 'ls -la | grep "."'
exec_test 'ls hola'
exec_test 'ls | exit'
exec_test 'ls | ls hola'
exec_test 'ls | ls | echo hola | rev'
exec_test 'pwd'
exec_test 'pwd hola'
exec_test 'pwd hola que tal'
exec_test 'pwd ls'
exec_test 'pwd ls env'
exec_test 'pwd pwd pwd'
exec_test 'pwd | echo hola'
#exec_test 'sleep 1 | exit'
#exec_test 'sleep 1 | sleep 1'
exec_test 'unset $HOLA'
exec_test 'unset HOL$?A'
#exec_test 'unset HOLA | cat -e'
exec_test 'unset HOLA9'
exec_test 'unset HOLA_'
exec_test 'unset HOL_A'
exec_test 'unset INEXISTANT'
exec_test 'unset _HOLA'
exec_test 'unset _______'
exec_test 'unset cd'
exec_test 'unset echo'
exec_test 'unset export'
exec_test 'unset pwd'
exec_test 'unset sudo'
exec_test 'unset unset'
exec_test 'whoami | grep $USER'

# More tests 
exec_test '< Makefile grep main'
exec_test '< src/main.c < eval.sh < Makefile grep main'
exec_test '< eval.sh grep main < src/main.c < Makefile'
exec_test '< "Makefile" grep main'
exec_test '< "Makefile " grep main'
exec_test '"<" Makefile grep main'
exec_test '$?'
exec_test 'echo $?'

## Redirection Tests that work
	# exec_test '<<stop grep hallo >out.txt'
	# exec_test 'ls >> out.txt'
	# exec_test 'ls >>out.txt'
	# exec_test 'ls >> "out.txt"'
	# exec_test 'ls >>"out.txt"'


## Tests die Fehler verursachen
	# exec_test 'ls>out.txt'
	# exec_test 'ls>>out.txt'
	# exec_test 'ls>> out.txt'
	# exec_test '<<stop grep hallo>>out.txt'
	# exec_test 'grep main<Makefile>out.txt'
	# exec_test '<Makefile grep main>out.txt'
	# exec_test 'grep main<"Makefile">out.txt'
	# exec_test '""echo hola"" que"" tal""'
	# exec_test '> pwd'
	# exec_test 'cat <pwd'
	# exec_test 'cd ~/'
	# exec_test 'echo hola""'
	# exec_test 'echo hola""""""""""""'
	# exec_test 'echo hola""bonjour'
	# exec_test 'echo \$HOME'
	# exec_test 'echo \n hola'
	# exec_test 'echo $USER$var\$USER$USER\$USERtest$USER'
	# exec_test 'echo $?$'
	# exec_test 'echo $"HOME"'
	# exec_test 'echo $""'
	# exec_test 'echo $""HOME'
	# exec_test 'echo $"HO""ME"'
	# exec_test 'echo $"HOLA"'
	# exec_test 'echo "$HO"ME'
	# exec_test 'echo "$HO""ME"'
	# exec_test 'echo ""ME'
	# exec_test 'echo ""hola'
	# exec_test 'echo "$"""'
	# exec_test 'echo "$"HOME'

## Funktionierende Tests (mit minimalen Unterschieden in der Ausgabe oder beabsichtigter anderer Ausgabe)

#	exec_test '/usr/bin/env'
#	exec_test '   		env'
#	exec_test 'env'
#	exec_test "     '$env'    "
#	exec_test '	export  '
#	exec_test '	export     M=42 '
#	exec_test 'cat | echo || ls'
#	exec_test 'pwd || ls'
#	exec_test 'pwd || ls || echo hola'
#	exec_test 'ls || cat | echo'
#	exec_test 'ls || export ""'
#	exec_test 'echo hola || cat'
#	exec_test 'echo hola || echo bonjour'
#	exec_test 'echo bonjour || echo hola'
#	exec_test 'cat M*ee'
#	exec_test 'echo $*'
#	exec_test 'echo D*'
#	exec_test 'echo *Z'
#	exec_test 'export HOLA=bon!jour'
#	exec_test 'ls *.hola'
#	exec_test '(< pwd)'
#	exec_test '/bin/doesnotexist -doesnotexist'

# # PREPARE AND COMPARE OUTPUT
 gsed -i 's/ line 1: / / g' out_bash.txt
 diff -y out_bash.txt out_minishell.txt >> out_diff.txt
