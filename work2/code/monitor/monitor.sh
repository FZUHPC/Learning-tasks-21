#!/bin/bash
filename="monitor.out" # 定义输出文件
divideline="######################################################################################"

while(true)
do
    nowdate=$(date +"%Y-%m-%d %H:%M:%S")                            # 获取日期
    cpu_free=`top -b -n 1 | grep Cpu | awk '{print $8}'`            # 获取CPU闲置率
    cpu_usage=`echo "100.0 - $cpu_free" | bc`                       # 获取CPU使用率
    processes=`ps -aux | sort -k3nr | head -n 3 | awk '{print $2}'` # 获取占用率前十进程PID和利用率

    cat /dev/null > $filename                             # 清空
    echo "$nowdate CPU_USAGE_RATE: $cpu_usage%\n" >> $filename # 输出CPU使用率

    for process_id in ${processes}
    do
        echo "PID [$process_id] INFO" >> $filename
        echo $divideline >> $filename
        echo "  PID   USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND" >> $filename
        top -b -n 1 -p $process_id | grep $USER >> $filename
        echo $divideline >> $filename

        echo "  PID   USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND" >> $filename
        top -H -b -n 1 -p $process_id | grep $USER >> $filename
        echo $divideline >> $filename
        echo "\n\n" >> $filename
    done

    sleep 120
done