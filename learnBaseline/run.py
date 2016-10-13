import os
import subprocess
from GetResult import *

first_run = 'vw -d data_by_days_VW/21.txt --adaptive --invariant --l2 0.8e-8 -f 2.model --loss_function logistic -c --save_resume -b 20'
a = 'vw -d data_by_days_VW/21.txt -i 2.model --save_resume -c -f 3.model'.split()
for i in range(5):
	subprocess.call('vw -d data_by_days_VW/21.txt -i 2.model --save_resume -c -f 2.model -p 21'.split())
	last_MAP = GetMAP()
better_MAP = True
n_epoch = 0
for i in range(20):
	better_MAP = True
	n_epoch = 0
	print('IIII ', i)
	while (better_MAP):
		n_epoch += 1
		print n_epoch
		if (n_epoch >= 10):
			break
		a[2] = 'data_by_days_VW/' + str(20 - i) + '.txt'
		print(a)
		print 'Test1'
		subprocess.call(a)
		print 'Test'
		subprocess.call('vw -d data_by_days_VW/21.txt -i 3.model --save_resume -c -f 4.model -p 21'.split())
		MAP = GetMAP()
		print MAP, last_MAP
		if(last_MAP > MAP):
			better_MAP = False
		if (better_MAP):
			subprocess.call('mv 3.model 2.model'.split())
			last_MAP = MAP
		print better_MAP
		

#subprocess.call(['vw', '-d', 'data_by_days_VW/1.txt', '-i', '1.model', '--save_resume'])