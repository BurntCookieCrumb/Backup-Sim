#!/bin/bash
#date "+DATE: %Y%m%d_%H%M%S"

OPT=$1

echo ""
echo " -- start postprocessing --"
echo ""

if [[ "$OPT" == all ]]; then
    root -l -b -q ppData.C++\(\)
    root -l -b -q ppSim.C++\(\)
fi

if [[ "$OPT" == data ]]; then
    root -l -b -q ppData.C++\(\)
fi

if [[ "$OPT" == sim ]]; then
    root -l -b -q ppSim.C++\(\)
fi

root -l -b -q ppResults.C++\(\)

rm *.so
rm *.d
rm *.pcm

echo ""
echo " -- done processing $OPT --"
echo ""

