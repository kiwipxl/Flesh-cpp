from __future__ import print_function;
import time;

P_INFO = 0;         #prints information, such as a client being accepted
P_MID = 1           #prints MID data
P_WARNING = 2;      #prints an information warning, these do not need to be fixed and will occur frequently
P_ERROR = 3;        #prints a high priority error, which when occurs, most likely will need fixing
P_FATAL = 4;        #crashes the program and produces a fatal error which must be fixed

P_INFO_TITLE = "INFO";
P_MID_PRINT = "MID";
P_WARNING_TITLE = "WARNING";
P_ERROR_TITLE = "ERROR";
P_FATAL_TITLE = "!FATAL!";

titles = [P_INFO_TITLE, P_MID_PRINT, P_WARNING_TITLE, P_ERROR_TITLE, P_FATAL_TITLE];
start_time = time.time();

def log(log_msg, priority, new_line = '\n'):
    if (priority >= 0 and priority <= P_FATAL):
        print("[%s (%.2f)]: %s" % (titles[priority], (time.time() - start_time), log_msg), end=new_line);