#!/usr/sbin/dtrace -qs


/*
 ** By doing dtrace -l -f open we get:
 * ID   PROVIDER            MODULE                          FUNCTION NAME
 * 22    syscall           vmlinux                              open entry
 * 23    syscall           vmlinux                              open return
 *
 ** From the man page:
 * int open(const char *pathname, int flags, mode_t mode);
 *
 * The argument flags must include one of the following access modes:
 *      O_RDONLY, O_WRONLY, or O_RDWR. 
 *
 * ...
 *
 * The return value of open() is a file descriptor, a small, nonnegative
 *     integer that is used in subsequent system calls (read(2), write(2),
 *     lseek(2), fcntl(2), etc.) to refer to the open file.  The file
 *     descriptor returned by a successful call will be the lowest-numbered
 *     file descriptor not currently open for the process.
 */

syscall::open:entry
{
    fullpath = copyinstr(arg0);
    wantedpath = "/etc/";
    isPath = strstr(fullpath, wantedpath);
}

syscall::open:return
/ isPath != NULL /
{
    mask = 3;
    accessFlag = "ERROR"; /* Impossible case */
    accessFlag = (arg1 & mask) == O_RDONLY ? "O_RDONLY" : accessFlag;
    accessFlag = (arg1 & mask) == O_WRONLY ? "O_WRONLY" : accessFlag;
    accessFlag = (arg1 & mask) == O_RDWR   ? "O_RDWR"   : accessFlag;

    appendFlag = arg1 == O_APPEND ? "APPEND " : "";
    createFlag = arg1 == O_CREAT  ? "CREATE " : "";

    printf("Process %s:\n", execname);
    printf("PID:\t%d\n", pid);
    printf("UID:\t%d\n", uid);
    printf("GID:\t%d\n", gid);
    printf("Path:\t%s\n", fullpath);
    printf("Probe:\t%s\n", probefunc);
    printf("Access mode:\t%s\n", accessFlag);
    printf("Optional flags:\t%s%s\n", appendFlag, createFlag); 

    printf("Return value: %d\n", arg0);
    printf("\n");
}