#! /bin/sh
#===============================
#
#          cpulist.sh
#
# 2008/11 morimoto
#===============================

rm cpulist.c 1>/dev/null 2>&1 
grep -h INSTALL_CPU $* | sed -e "s/INSTALL_CPU/CPU_EXTERN/g" >> cpulist.c
echo "static const STCPU *s_ptblCpuList[] = {"               >> cpulist.c
grep -h INSTALL_CPU $* | sed -e "s/INSTALL_CPU/CPU_DEFINE/g" >> cpulist.c
echo "};"                                                    >> cpulist.c
echo "CPU_LIST_SIZE;"                                        >> cpulist.c
