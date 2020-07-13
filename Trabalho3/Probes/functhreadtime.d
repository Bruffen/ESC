#!/usr/sbin/dtrade -s
 
pid$target:$1::entry
{
	ts[probefunc,tid] = timestamp;
	@func[probefunc,tid] = count();
}

pid$target:$1::return
/ts[probefunc,tid]/
{
	@ft[probefunc,tid] = sum(timestamp - ts[probefunc,tid]);
	ts[probefunc,tid] = 0;
}

END
{
	printf("%20s %5s %10s %10s\n", "FUNCTION", "TID", "CALLS", "TIME SPENT");
	printa("%20s %5u %10@u %10@u\n",@func,@ft);
}
