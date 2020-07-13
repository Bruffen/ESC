pid$target::MPI*:entry
{
	@c[probefunc] = count();
	ts[probefunc] = timestamp;
}

pid$target::MPI*:return
{
	@t[probefunc] = sum(timestamp - ts[probefunc]);
	@c[probefunc] = count();
}

END
{
	printf("%20s %10s %s\n,","MPI Func", "Calls", "Time Spent");
	printa("%20s %10@u %@u\n",@c,@t);
}
