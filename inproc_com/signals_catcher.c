#include <stdio.h>
#include <signal.h>


void sig_handler(int signum)
{
    switch (signum)
    {
        case SIGHUP:    printf("\nCatch signal: SIGHUP\n");     break;
        case SIGINT:    printf("\nCatch signal: SIGINT\n");     break;
        case SIGQUIT:   printf("\nCatch signal: SIGQUIT\n");    break;
        case SIGILL:    printf("\nCatch signal: SIGILL\n");     break;
        case SIGTRAP:   printf("\nCatch signal: SIGTRAP\n");    break;
        case SIGABRT:   printf("\nCatch signal: SIGABRT\n");    break;
        case SIGBUS:    printf("\nCatch signal: SIGBUS\n");     break;
        case SIGFPE:    printf("\nCatch signal: SIGFPE\n");     break;
        case SIGKILL:   printf("\nCatch signal: SIGKILL\n");    break;
        case SIGUSR1:   printf("\nCatch signal: SIGUSR1\n");    break;
        case SIGSEGV:   printf("\nCatch signal: SIGSEGV\n");    break;
        case SIGUSR2:   printf("\nCatch signal: SIGUSR2\n");    break;
        case SIGPIPE:   printf("\nCatch signal: SIGPIPE\n");    break;
        case SIGALRM:   printf("\nCatch signal: SIGALRM\n");    break;
        case SIGTERM:   printf("\nCatch signal: SIGTERM\n");    break;
        case SIGSTKFLT: printf("\nCatch signal: SIGSTKFLT\n");  break;
        case SIGCHLD:   printf("\nCatch signal: SIGCHLD\n");    break;
        case SIGCONT:   printf("\nCatch signal: SIGCONT\n");    break;
        case SIGSTOP:   printf("\nCatch signal: SIGSTOP\n");    break;
        case SIGTSTP:   printf("\nCatch signal: SIGTSTP\n");    break;
        case SIGTTIN:   printf("\nCatch signal: SIGTTIN\n");    break;
        case SIGTTOU:   printf("\nCatch signal: SIGTTOU\n");    break;
        case SIGURG:    printf("\nCatch signal: SIGURG\n");     break;
        case SIGXCPU:   printf("\nCatch signal: SIGXCPU\n");    break;
        case SIGXFSZ:   printf("\nCatch signal: SIGXFSZ\n");    break;
        case SIGVTALRM: printf("\nCatch signal: SIGVTALRM\n");  break;
        case SIGPROF:   printf("\nCatch signal: SIGPROF\n");    break;
        case SIGWINCH:  printf("\nCatch signal: SIGWINCH\n");   break;
        case SIGIO:     printf("\nCatch signal: SIGIO\n");      break;
        case SIGPWR:    printf("\nCatch signal: SIGPWR\n");     break;
        case SIGSYS:    printf("\nCatch signal: SIGSYS\n");     break;
        default:        printf("\nCatch UNKNOWN signal\n");
    }
}


void init_signal_handler(void)
{
    signal(SIGHUP, sig_handler);    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);   signal(SIGILL, sig_handler);
    signal(SIGTRAP, sig_handler);   signal(SIGABRT, sig_handler);
    signal(SIGBUS, sig_handler);    signal(SIGFPE, sig_handler);
    signal(SIGKILL, sig_handler);   signal(SIGUSR1, sig_handler);
    signal(SIGSEGV, sig_handler);   signal(SIGUSR2, sig_handler);
    signal(SIGPIPE, sig_handler);   signal(SIGALRM, sig_handler);
    signal(SIGTERM, sig_handler);   signal(SIGSTKFLT, sig_handler);
    signal(SIGCHLD, sig_handler);   signal(SIGCONT, sig_handler);
    signal(SIGSTOP, sig_handler);   signal(SIGTSTP, sig_handler);
    signal(SIGTTIN, sig_handler);   signal(SIGTTOU, sig_handler);
    signal(SIGURG, sig_handler);    signal(SIGXCPU, sig_handler);
    signal(SIGXFSZ, sig_handler);   signal(SIGVTALRM, sig_handler);
    signal(SIGPROF, sig_handler);   signal(SIGWINCH, sig_handler);
    signal(SIGIO, sig_handler);     signal(SIGPWR, sig_handler);
    signal(SIGSYS, sig_handler);    signal(SIGRTMIN, sig_handler);
}

int main(void)
{
    init_signal_handler();

    while (1) { }

    return 0;
}