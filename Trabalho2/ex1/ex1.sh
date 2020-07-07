#!/bin/sh

echo "cat /etc/inittab > /tmp/test
"
dtrace -qs open.d -c "cat /etc/inittab > /tmp/test"
echo "____________________________________________
"

echo "cat /etc/inittab >> /tmp/test
"
dtrace -qs open.d -c "cat /etc/inittab >> /tmp/test"
echo "____________________________________________
"

echo "cat /etc/inittab | tee /tmp/test
"
dtrace -qs open.d -c "cat /etc/inittab | tee /tmp/test"
echo "____________________________________________
"

echo "cat /etc/inittab | tee -a /tmp/test
"
dtrace -qs open.d -c "cat /etc/inittab | tee -a /tmp/test" 
echo "____________________________________________
"
