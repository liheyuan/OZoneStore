import os
import sys
import time
import subprocess

if __name__ == "__main__":
	keyfile =  sys.argv[1]
	tmpfile =  "/tmp/tmp_%d" % (int(time.time()))
	if os.system("sort -k1n -k2nr %s | sort -u -k1n -k2n,1 > %s" %(keyfile ,tmpfile)) !=0:
		sys.exit(-1)
	cmd = '/usr/bin/wc -l %s' % (tmpfile)
	p = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE)
	print p.communicate()[0].split()[0]
	sys.stdout.flush()
	p.wait()
	cmd = "cat %s" % (tmpfile)
	p = subprocess.Popen(cmd.split())
	p.wait()

