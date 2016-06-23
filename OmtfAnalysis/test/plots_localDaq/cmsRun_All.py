#!/usr/bin/python

import subprocess
 

#run = 'run_0_2016_5_15__1_11_26.txt'
#run = 'run_0_2016_5_7__18_23_22.txt'
run = 'run_0_2016_6_13__20_12_14.txt'

OMTF = ['OMTFn1','OMTFn2','OMTFn3','OMTFn4','OMTFn5','OMTFn6','OMTFp1','OMTFp2','OMTFp3','OMTFp4','OMTFp5','OMTFp6'] 

for board in OMTF:
  file = board+'_'+run
  subprocess.call(['cmsRun','../localDaq.py',file])

