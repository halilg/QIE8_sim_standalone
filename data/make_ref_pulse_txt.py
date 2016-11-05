#!/usr/bin/env python
import os, sys
#   TH1F *h_1_in__3 = new TH1F("h_1_in__3","h_1_in",500,-0.125,124.875

if len(sys.argv) < 2: sys.exit()

ifname=sys.argv[1]



dt=(124.875+0.125)/500
i=0

ls=file(ifname).readlines()

i=0
print 0,0
for l in ls:
    if not "SetBinContent" in l: continue
    l=l.strip().split(",")[1]
    l=l.replace(");","")
    i=i+1
    print i*dt,l
    
