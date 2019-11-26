#include <sys/cdefs.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

__BEGIN_DECLS

extern int test_main(void);

#ifndef SINGLE_TEST
extern int bzero_tests(void);
extern int ctype_tests(void);
extern int memcpy_tests(void);
extern int memset_tests(void);
extern int puts_tests(void);
extern int strcat_tests(void);
extern int strdup_tests(void);
extern int strlen_tests(void);
// Bonus functions
extern int strstr_tests(void);
extern int atoi_tests(void);
extern int memcmp_tests(void);
extern int memmove_tests(void);
extern int rdtsc_tests(void);
extern int strspn_tests(void);
extern int strcspn_tests(void);
extern int debugbreak_tests(void);
const char *signames[] = {
[0] = "UNKNOWN",
[SIGHUP] = "SIGHUP",
[SIGINT] = "SIGINT",
[SIGQUIT] = "SIGQUIT",
[SIGILL] = "SIGILL",
[SIGTRAP] = "SIGTRAP",
[SIGABRT] = "SIGABRT",
[SIGEMT] = "SIGEMT",
[SIGFPE] = "SIGFPE",
[SIGKILL] = "SIGKILL",
[SIGBUS] = "SIGBUS",
[SIGSEGV] = "SIGSEGV",
[SIGSYS] = "SIGSYS",
[SIGPIPE] = "SIGPIPE",
[SIGALRM] = "SIGALRM",
[SIGTERM] = "SIGTERM",
[SIGURG] = "SIGURG",
[SIGSTOP] = "SIGSTOP",
[SIGTSTP] = "SIGTSTP",
[SIGCONT] = "SIGCONT",
[SIGCHLD] = "SIGCHLD",
[SIGTTIN] = "SIGTTIN",
[SIGTTOU] = "SIGTTOU",
[SIGIO] = "SIGIO",
[SIGXCPU] = "SIGXCPU",
[SIGXFSZ] = "SIGXFSZ",
[SIGVTALRM] = "SIGVTALRM",
[SIGPROF] = "SIGPROF",
[SIGWINCH] = "SIGWINCH",
[SIGINFO] = "SIGINFO",
[SIGUSR1] = "SIGUSR1",
[SIGUSR2] = "SIGUSR2",
};
void sig_handler(int signo) {
  if (signo >= 0 && signo < 32)
    printf("Received %s\n", signames[signo]);
}
int test_main(void) {
  int result = 0;
  result |= bzero_tests();
  result |= ctype_tests();
  result |= memcpy_tests();
  result |= memset_tests();
  result |= puts_tests();
  result |= strcat_tests();
  result |= strdup_tests();
  result |= strlen_tests();
  result |= strstr_tests();
  result |= atoi_tests();
  result |= memcmp_tests();
  result |= memmove_tests();
  result |= rdtsc_tests();
  result |= strspn_tests();
  result |= strcspn_tests();
  if (signal(SIGTRAP, sig_handler) == SIG_ERR)
    printf("\ncannot catch SIGTRAP, expect debugbreak_tests to kill the program\n");
  result |= debugbreak_tests();
  return result;
}
#endif
int main() { return test_main(); }

__END_DECLS
