#!/bin/bash

# check if stdout is a terminal...
if [ -t 1 ] && command tput 2>/dev/null; then

    # see if it supports colors...
    ncolors=$(tput colors)

    if test -n "$ncolors" && test $ncolors -ge 8; then
        bold="$(tput bold)"
        underline="$(tput smul)"
        standout="$(tput smso)"
        normal="$(tput sgr0)"
        black="$(tput setaf 0)"
        red="$(tput setaf 1)"
        green="$(tput setaf 2)"
        yellow="$(tput setaf 3)"
        blue="$(tput setaf 4)"
        magenta="$(tput setaf 5)"
        cyan="$(tput setaf 6)"
        white="$(tput setaf 7)"
    fi
fi

# cd to the directory and run the compiler for every input file
# then check if the output matches the desired output
cd tests
for file in `ls *.ul`; do
  if [ `basename $file` != 'test_runner.sh' ]; then
    ../binaries/tulc.exe < $file > `basename $file`.result
    if [ "`diff $file.result $file.correct`" != "" ]; then
      echo "${red}Test with $file failed${normal}"
    else
      echo "${green}Test with $file succeeded${normal}"
      rm "$file.result"
    fi
  fi
done
