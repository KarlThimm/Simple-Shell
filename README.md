# Simple-Shell
This is a custom shell that is able to run simple commands.
exit - obvious!
which - same as the tcsh one (hint: you can write a function to do this and use it for finding the command to execute) [Review get_path_main.c in shell skeleton code for details]
where - same as the tcsh one (reports all known instances of command in path) [Review get_path_main.c in shell skeleton code]
cd - chdir(2) to the directory given; with no arguments, chdir to the home directory; with a '-' as the only argument, chdirs to directory previously in, the same as what tcsh does. [Review Stevens and Rago's Section 4.23 for details and the reason why cd has to be implemented as a built-in command]
pwd - print the current working directory. [Review getcwd.c in sample code; review Stevens and Rago's Section 4.23 for details]
list - with no arguments, lists the files in the current working directory one per line. With arguments, lists the files in each directory given as an argument, with a blank line then the name of the directory followed by a : before the list of files in that directory. You will need to use opendir(3) and readdir(3). (Hint: read their respective man pages carefully, and refer to Fig. 1.3 of Stevens and Rago's APUE book)
pid - prints the pid of the shell
kill - When given just a pid, sends a SIGTERM to the process with that pid using kill(2). When given a signal number (with a - in front of it), sends that signal to the pid. (e.g., kill 5678, kill -9 5678). Read the man page of kill(2) to check for return value of -1 on error and the 'errno' value of ESRCH (3) for "No such process."
prompt - When ran with no arguments, prompts for a new prompt prefix string. When given an argument make that the new prompt prefix. For instance, let's assume cwd is /usa/cshen.
 [/usa/cshen]> prompt CISC361
CISC361 [/usa/cshen]> _
CISC361 [/usa/cshen]> cd 361
CISC361 [/usa/cshen/361]> prompt YES
YES [/usa/cshen/361]> prompt
  input prompt prefix: hello
hello [/usa/cshen/361]> _
printenv - Should work the same as the tcsh built-in one. When ran with no arguments, prints the whole environment. (This can be done in 2 lines of code, a printf() inside of a while loop by accessing the extern environ variable, not counting a variable declaration). When ran with one argument, call getenv(3) on it. When called with two or more args, print the same error message to stderr that tcsh does. (Review Stevens and Rago's Section 7.5 Environment List [and Section 7.4 Command-Line Arguments] for details.)
setenv - Should work the same as the tcsh built-in one. When ran with no arguments, prints the whole environment, the same as printenv. When ran with one argument, sets that as an empty environment variable. When ran with two arguments, the second one is the value of the first. When ran with more args, print the same error message to stderr that tcsh does. You can use the setenv(3) function for this command. Special care must be given when PATH and HOME are changed. When PATH is changed, be sure to update your linked list for the path directories (and free() up the old one). When HOME is changed, cd with no arguments should now go to the new home.
