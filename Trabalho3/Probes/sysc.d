#!/usr/sbin/dtrace -qs
this int ts;

BEGIN
{
        runtime = timestamp;
}

syscall:::entry
/pid == $target/
{
        @counts[probefunc] = count();
        ts[probefunc] = timestamp;
        totalcounts++;
}

syscall:::return
/pid == $target && ts[probefunc]/
{
        @avgtime[probefunc] = avg(timestamp - ts[probefunc]);
        @totaltime[probefunc] = sum(timestamp - ts[probefunc]);
        @percentage[probefunc] = sum(timestamp - ts[probefunc]);
        this->ts = 0;
}

syscall:::return
/pid == $target && errno != 0/
{
        @errors[probefunc] = count();
        totalerrors++;
}

END
{
        printf("%% %4s %8s %10s %5s %6s %-s\n","time","seconds","usecs/call","calls",
"errors","syscall");
        runtime = timestamp - runtime;
        normalize(@avgtime,1);
        normalize(@totaltime,1);
        normalize(@percentage,runtime/100);
        printa("%6@u %@8u %@10u %@5u %@6u %-s\n", @percentage,
@totaltime,@avgtime, @counts,@errors,@totaltime);
        printf("%6s %8d %10s %5u %6u %-s","100.00",runtime,"",
totalcounts,totalerrors,"total");
}
