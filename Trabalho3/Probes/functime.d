#!/usr/sbin/dtrade -s

 
pid$target:$1::entry
{
	ts[probefunc] = timestamp;
	@func[probefunc] = count();
}

pid$target:$1::return
/ts[probefunc]/
{
	@ft[probefunc] = sum(timestamp - ts[probefunc]);
	ts[probefunc] = 0;
}

END
{
	printf("%20s %10s %10s\n", "FUNCTION", "CALLS", "TIME SPENT");
	printa("%20s %10@u %10@u\n",@func,@ft);
}
