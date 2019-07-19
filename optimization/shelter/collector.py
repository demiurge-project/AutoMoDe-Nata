import re
import os
import sys

name, remotefolder = sys.argv[1], os.getcwd()+"/execution-folder/"
t = re.compile('{}-(\d\d)'.format(name))
m = re.compile(r"# Best configurations as commandlines \(first number is the configuration ID\)\n\d*  (.*)")

f2 = []
for i in os.listdir(remotefolder):
    if t.match(i):
        print(i)
        f = open("{}/{}/irace.stdout".format(remotefolder, i), "r")
        g = f.read()
        fi = m.search(g)
        print(fi.group(1))
        f2 += [fi.group(1)]
out = open(name+"_fsm.txt","w")
out.write("-----------------------------------------------------------------\n")
for i in f2:
    out.write(i+"\n")
    out.write("-----------------------------------------------------------------\n")
out.close()
