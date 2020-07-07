syscall::open*:entry
/(arg2 & O_CREAT) == O_CREAT/
{
	@open[pid, execname] = count();
}

syscall::open*:entry
/(arg2 & O_CREAT) == 0/
{
	@existing[pid, execname] = count();
}

syscall::open*:return
/arg1 > 0/
{
	@success[pid, execname] = count();
}

tick-$1s
{
	printf("%Y\n", walltimestamp);
	printf("%6s %12s %6s %6s %s\n","PID","EXECNAME","CREATED","OPENED","SUCCESSFUL");
	printa("%6d %12s %@6d %@6d %@d\n",@open,@existing,@success);
	trunc(@open);
	trunc(@existing);
	trunc(@success);
}
