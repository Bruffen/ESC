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

    flags = arg1;
}

syscall::open:return
/ isPath != NULL /
{
    accessFlag = flags & O_WRONLY ? "O_WRONLY" 
        : flags & O_RDWR ? "O_RDWR" 
            : "O_RDONLY",

    appendFlag = flags & O_APPEND ? "|O_APPEND" : "";
    createFlag = flags & O_CREAT  ? "|O_CREAT"  : "";
    allFlags = strjoin(accessFlag, strjoin(appendFlag, createFlag));

    printf("Process %s:", execname);
    printf("\tPID:%d,", pid);
    printf("\tUID:%d,", uid);
    printf("\tGID:%d\n", gid);
    printf("Probe:\t%s (path: %s, flags: %s)\n", probefunc, fullpath, allFlags);
    printf("Return: %d\n", arg0);
    printf("_________________________________________\n");
}