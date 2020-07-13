
BEGIN
{
	iteration = 0;
}

dt*:::disttrans-programstart
{
	timestart = timestamp;
	total = 0;
}

dt*:::disttrans-itstart
{
	self->t = timestamp;
}

dt*:::disttrans-itend
{
	iteration++;
	t = timestamp;
	printf("%s (pid=%d) spent %d microseconds in iteration %d\n",
execname, pid, ((t - self->t)/1000), iteration);
	total += t - self->t;
	self->t = 0;
}

dt*:::disttrans-loadstart
{
	load_t = timestamp;
}

dt*:::disttrans-loadend
{
	load_t = timestamp - load_t;
}

dt*:::disttrans-savestart
{
	save_t = timestamp;
}

dt*:::disttrans-saveend
{
	save_t = timestamp - save_t;
}



dt*:::disttrans-programend
{
	totaltime = timestamp - timestart;
	p_save = (save_t*100)/totaltime;
	p_load = (load_t*100)/totaltime;
	p_it = (total*100)/totaltime;

	save_t = save_t/1000;
	load_t = load_t/1000;
	total = total/1000;
	totaltime = totaltime /1000;
	average = total/iteration;
	
	printf("%10s %11s %10s %5s %s\n", "FUNCTION","TIME SPENT","PERCENTAGE","CALLS","TIME/CALL");
	printf("%10s %11d %10d %5d %d\n", "LOAD",load_t, p_load, 1, load_t);
	printf("%10s %11d %10d %5d %d\n", "TRANSFORM", total, p_it, iteration, average);
	printf("%10s %11d %10d %5d %d\n", "SAVE", save_t, p_save, 1, save_t);
	printf("%10s %11d %10d","TOTAL TIME", totaltime, 100);
}


