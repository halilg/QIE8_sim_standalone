#!/usr/bin/env python
import sys
#   TH1F *h_1_in__3 = new TH1F("h_1_in__3","h_1_in",500,-0.125,124.875

ls=file("ref_pulse.cc").readlines()

dt=(124.875+0.125)/500
i=0

integral=0
#integrate:
for l in ls:
    i=i+1
    integral=integral+dt*float(l.strip().split(",")[1])


i=0
print 0,0
for l in ls:
    i=i+1
    print i*dt,float(l.strip().split(",")[1])/integral
    
while i*dt < 749.5:
    i=i+1
    print i*dt,0