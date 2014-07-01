#ifndef UTCEPH_H
#define	UTCEPH_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <librados.h>
#include <stdbool.h>
#include <libconfig.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <sys/syscall.h>
#include <stdarg.h>
#include <fcgiapp.h>
#include <getopt.h>
#include <dirent.h>
#include <pwd.h>    
#include <grp.h>
#include <sys/time.h>
#define __USE_GNU
#include <sys/resource.h>

#define FREE(ptr) do{ \
    free((ptr));      \
    (ptr) = NULL;     \
  }while(0)

#define BUFFSZ 4194304
#define MB 1048576
#define GB 1073741824
#define UMIN(a,b) ((a)>(b)?(b):(a))
#define CFG_PARAM_LEN 32
#define MAXPOOLS 65535

    bool exitflag = false;
    static const struct option longOpts[] = {
        { "config", required_argument, NULL, 'c'},
        { "ceph-config", required_argument, NULL, 'f'},
        { NULL, no_argument, NULL, 0}
    };
    int opt = 0;
    int longIndex = 0;
    struct passwd *pwd;
    struct group *grp;
    pid_t pid, sid;

    uid_t my_uid;
    gid_t my_gid;

    pthread_t id[MAXPOOLS];
    
    const char ok[] = "Status: 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n";
    const char deleted[] = "Status: 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nDeleted<br />\n";

    const char created[] = "Status: 201 Created\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nCreated<br />\n";

    const char not_found[] = "Status: 404 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nNot found<br />\n";

    const char not_supported[] = "Status: 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nNot supported<br />\n";

    const char forbidden[] = "Status: 403 Forbidden\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nForbidden<br />\n";

    const char usage[] = "Status: 200 OK\r\n"
            "Content-type: text/html\r\n\r\n"
            "No parameters given<br />\n"
            "Upload: curl -XPUT -T filename http://uploadhost.com/poolname/filename<br />\n"
            "Download: curl -XGET http://uploadhost.com/poolname/filename<br />\n"
            "Delete: curl -XDELETE http://uploadhost.com/poolname/filename<br />\n"
            "Stat: curl -XOPTIONS http://uploadhost.com/poolname/filename<br />\n";

    const char no_data[] = "Status: 200 OK\r\n"
            "Content-type: text/html\r\n\r\n"
            "No data from standard input\n";

    const char ierror[] = "Status: 500 Internal Error\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\nInternal error<br />\n";

    const char program[] = "RadosWeb";
    FILE * pid_fd = NULL;
    FILE * logger_fd = NULL;

    typedef struct {
        time_t c_time;
        time_t m_time;
        uint64_t f_size;
        char extension[8];
        char mime[32];
    } meta;

    struct {
        int logging;
        int foreground;
        char *user;
        char *group;
        char *logfile;
        char *pidfile;
        char *socket;
        char *configfile;
        char *ceph_config;
        char *ceph_user;
        int threads_count;
        char **pools;
    } globals;

    struct _current_data {
        char filename[256];
        char poolname[32];
        char method[8];
        char requri[256];
        char qstring[8];
        struct rusage usage;
        long int cLength;
        meta metadata;
    } current_data[MAXPOOLS];

    int listenfd;

#ifdef	__cplusplus
}
#endif

#endif	/* UTCEPH_H */
