#pragma D option dynvarsize=800000
#pragma D option cleanrate=50hz
#pragma D option bufpolicy=ring

lockstat:::adaptive-acquire
/pid == $target/
{
        @locks[execname,tid,"adaptive"] = count();
        ts["adaptive"] = timestamp;
}


lockstat:::adaptive-release
/pid == $target/
{
        @time[execname,tid,"adaptive"] = sum(timestamp - ts["adaptive"]);
}

lockstat:::spin-acquire
/pid == $target/
{
        @locks[execname,tid,"spin"] = count();
        ts["spin"] = timestamp;
}


lockstat:::spin-release
/pid == $target/
{
        @time[execname,tid,"spin"] = sum(timestamp - ts["spin"]);
}

lockstat:::rw-acquire
/pid == $target/
{
        @locks[execname,tid,"rw"] = count();
        ts["rw"] = timestamp;
}


lockstat:::rw-release
/pid == $target/
{
        @time[execname,tid,"rw"] = sum(timestamp - ts["rw"]);
}


END
{
        normalize(@time, 1000);
        printf("%20s %10s %12s %10s %15s\n", "FUNCTION", "TID", "PROBENAME", "CALLS", "TIMESPENT");
        printa("%20s %10d %12s %10@u %15@d\n",@locks, @time);
}
