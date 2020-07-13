proc:::lwp-create
{
	printf("%s, %d\n", stringof(args[1]->pr_fname), args[0]->pr_lwpid);
}

proc:::lwp-start
/tid != 1/
{
	self->start = timestamp;
}

proc:::lwp-exit
/self->start/
{
	@[tid, stringof(curpsinfo->pr_fname)] = sum(timestamp - self->start);
	self->start = 0;
}

