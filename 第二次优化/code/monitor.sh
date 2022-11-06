#!/bin/bash
now=`date -u -d"+8 hour" +'%Y-%m-%d %H:%M:%S'`
#cpu报警阈值
cpu_warn='75'
#主代码
item_cpu () {

cpu_idle=`top -b -n 1 | grep Cpu | awk '{printf $8}'|cut -f 1 -d "."`

cpu_use=`expr 100 - $cpu_idle`

echo "$now 当前cpu使用率为 $cpu_use" >> /opt/cpu.log
        
        if [ $cpu_use -gt $cpu_warn ]
        then
                echo "cpu warning!"
        else
                echo "cpu ok!"
        fi

cpu_used=$(top -b -n1 | head -5 | grep ^%Cpu | awk '{printf("%.2f%%     "),100-$8}')    #已使用CPU百分比

top_proc_cpu=$(ps --no-headers -eo comm,pcpu | sort -k2 -n | tail -5)     #占用cpu最多的5个进程

echo -e "已使用CPU百分比：\033[32m$cpu_used\033[0m"

echo -e "占用CPU资源最多的5个进程列表：">>/opt/cpu.log

echo -e "$top_proc_cpu">>/opt/cpu.log

}

item_cpu
