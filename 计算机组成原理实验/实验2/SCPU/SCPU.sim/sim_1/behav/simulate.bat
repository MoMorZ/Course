@echo off
set xv_path=C:\\Xilinx\\Vivado\\2015.4\\bin
call %xv_path%/xsim test2_behav -key {Behavioral:sim_1:Functional:test2} -tclbatch test2.tcl -view C:/Users/HP/Desktop/ѧϰ/����/����ʵ��/ʵ���/CPU/SCPU/test2_behav.wcfg -log simulate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
