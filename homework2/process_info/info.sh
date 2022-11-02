#!/bin/bash

firstLine=0
lineCount=99

judgeFirstLine(){
	if [[ $* =~ "PID" ]];
	then
		 firstLine=1
	else
		 firstLine=0
	fi
	if [[ $* =~ "USER" ]];
	then
		 firstLine=1
	else
		 firstLine=0
	fi
	if [[ $* =~ "%CPU" ]];
	then
		 firstLine=1
	else
		 firstLine=0
	fi
	if [[ $* =~ "COMMAND" ]];
	then
		 firstLine=1
	else
		 firstLine=0
	fi
}

fileInit(){
	if [ -a process_info.txt ]
	then
		rm process_info.txt
	fi
	if [ -a thread_info.txt ]
	then
		rm thread_info.txt
	fi
	if [ -a tmp.txt ]
	then
		rm tmp.txt
	fi
}

fileDes(){
	rm tmp.txt
}

getProcessInfo(){
	top -b -n 5 -d 2 > tmp.txt
}

getThreadInfo(){
	if [[ $1 != "PID" ]]
	then
		echo >> thread_info.txt
		echo "PID $1:" >> thread_info.txt
		ps -mp $1 -o tid,pid,%cpu >> thread_info.txt
	fi
}
parse(){
	while read line
	do
	judgeFirstLine $line
		if [ $firstLine -eq 1 ]
		then
			echo >> process_info.txt
			lineCount=1
		else
			let "lineCount++"
		fi
		if [ $lineCount -ge 1 ] && [ $lineCount -le 4 ];then
			echo $line >> process_info.txt
			getThreadInfo $line
		fi
	done < tmp.txt
}

run(){
	fileInit
	getProcessInfo
	parse
	fileDes
}

run
