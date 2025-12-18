#!/bin/sh

rm -f /opt/amel/pid-control-pid
rm -f /opt/amel/temp-control-pid

/opt/amel/pid/main &
/opt/amel/temp-control/main &