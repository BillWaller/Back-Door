/* Author:  Bill Waller <BillWaller@wallerbroadcasting.com>
 * Copyright (c) 2015 Bill Waller
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <termio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/resource.h>

#ifndef PATH_MAX
#define PATH_MAX 256
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

void display_execvp_args(char *,char **);
void abend(char const *);

int
main(int argc, char **argv)
{
    struct group *GR;
    uid_t RealUid;
    int   F_daemon=FALSE,F_super=FALSE,F_debug=FALSE;

    if (argc<2) abend("usage: sg group-name|goroot args...");
    if      (!strncmp(argv[1],"Daemon",6)) F_daemon=TRUE;
    else if (!strncmp(argv[1],"goroot",6)) F_super =TRUE;
    else if ((GR=getgrnam(argv[1]))==NULL) {
        fprintf(stderr,"sg: invalid group-name: ");
        abend(argv[1]);
    }

    //---- Construct exec_path for shell ------------------------
    struct stat ss;
    char exec_path[PATH_MAX]="/bin/bash";
    char *p;
    if (p=getenv("SHELL"))
        if   (stat(p,&ss)!=-1) strcpy(exec_path,p);
        else if (stat(exec_path,&ss)==-1) abend("can't find shell");

    //---- Extract exec_file for argv[0] ------------------------
    char exec_file[PATH_MAX];
    char *s=p=exec_path;
    while (*p++); while (--p>s&&*p!='/'); p++;
    strcpy(exec_file,p); argv[0]=exec_file;

    //---- Shift args left to overlay command argument ------------
    int i=2;
    while (argv[i]) argv[i-1]=argv[i++];
    argv[i-1]=NULL;

    if (F_super || F_daemon) {
        //---- assume root identity -------------------------------
        if (seteuid(0)!=0) abend("unable to change user id (setuid?)");
        if (setegid(0)!=0) abend("unable to change group id (?)");
        if ( setuid(0)!=0) abend("unable to change user id");
        if ( setgid(0)!=0) abend("unable to change group id");
        struct rlimit rl;
        getrlimit(RLIMIT_FSIZE,&rl);
        rl.rlim_cur=RLIM_INFINITY; rl.rlim_max=RLIM_INFINITY;
        setrlimit(RLIMIT_FSIZE,&rl);
        putenv("LOGNAME=root"); putenv("USER=root");
        umask(0);
        if (F_debug) fprintf(stderr,"root identity assumed\n");
    }
    else {
        //---- assume group identity ------------------------------
        RealUid = getuid();
        if (RealUid&&seteuid(0))   abend("unable to change user id (setuid?)");
        if (setgid(GR->gr_gid)!=0) abend("sg: unable to set gid");
        if (seteuid(RealUid)!=0)   abend("sg: unable to set euid");
        if (F_debug) fprintf(stderr,"group identity assumed\n");
    }
    if (F_daemon) {
        //---- Dissociate terminal, close files -------------------
        setpgid(0,getgid());
        signal(SIGQUIT,SIG_IGN); signal(SIGINT ,SIG_IGN); signal(SIGHUP ,SIG_IGN);
        signal(SIGTTOU,SIG_IGN); signal(SIGTTIN,SIG_IGN); signal(SIGTSTP,SIG_IGN);
        if (F_debug) {
            fprintf(stderr,"daemon preparations\n");
            display_execvp_args(exec_path,argv);
	    }
        ioctl(2,TIOCNOTTY,0);
        int fd;
        for (fd=0;fd<20;fd++) close(fd);    
        chdir("/");
    }
    else if (F_debug) display_execvp_args(exec_path,argv);
    //---- Do the execvp ------------------------------------------
    execvp(exec_path,argv);
    fprintf(stderr,"Unable to execvp: %s\n",exec_path);
    exit(-1);
}


void display_execvp_args(char *s,char **argv)
{
    fprintf(stderr,"exec_path: %s\n",s);
    int i=0;
    while (*argv!=NULL&&**argv!='\0')
        fprintf(stderr,"argv[%d] %s\n",i++,*argv++);
}


void abend(char const *s)
{
    fprintf(stderr,"%s\n",s);
    exit(0);
}
