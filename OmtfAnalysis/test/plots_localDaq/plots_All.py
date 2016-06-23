#!/usr/bin/python

import subprocess
 

#run = 'run_0_2016_5_15__1_11_26'
#run = 'run_0_2016_5_7__18_23_22'
run = 'run_0_2016_6_13__20_12_14'
OMTF = ['n1','n2','n3','n4','n5','n6','p1','p2','p3','p4','p5','p6'] 

for board in OMTF:
  subprocess.call(['./plots_localDaq.py',board,run])

#done

