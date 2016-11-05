#!/usr/bin/env zsh
if ((! $+ROOTSYS)); then
  echo ROOTSYS is not defined
  exit
fi

#Let's make sure ROOT environment variables are properly defined
WDIR=$PWD
cd $ROOTSYS
source bin/thisroot.sh
cd $WDIR

UI=05
fC2uI() {
   [[ $CHARGE == 15 ]] && return
   (( UI = $CHARGE / 30 ))
}

TDECAY=0
TSTART=0
PLOTFILE=pulse.pdf

CHARGES=(15 30 60 150 300 600 1500 3000)
for CHARGE in $CHARGES; do
    fC2uI
    echo Current=$UI , charge=$CHARGE
    set -x
    ./Dump_Pulses $CHARGE $TDECAY $TSTART || exit
    ./plot_pulses pulse.txt pulse_preamp.txt data/h_${UI}_out.txt || exit
    set +x
done