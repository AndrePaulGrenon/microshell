42 School 4th exam: MICROSHELL

# microshell

SUPER EASY and SIMPLE MICROSHELL

This microshell is barely 121 lignes and could be optimize to be even lesser.

By using char **argv directly into the Execve System call there is no need to construct an array of arguments or malloc anything. No leaks possible.

This simple logic enables to focus mainly on the pipe aspect of the exam, error managing and the cd builtin. 

This code only uses 1 while loop statement in the main and 1 pipe at a time which makes it easy to follow:
1. Jumps to next arguments if command is ";" or "|" using continue;
2. Checks if builtin "cd" then continue;
3. Checks if the commands finishes with a pipe and opens it if true.
4. Fork and execute child process.
5. If command had a pipe, Dup2 the pipe in stdin to be used by next command.

Extra functions:
ft_strlen : Do I need to comment ?
EndsWithPipe: Returns true if command finishes with a pipe.
EndofCommand: Returns index of last argument relative to current command. Used by EndswithPipe to check next argument after last argument. Looks for ";" and  "|".
ErrorStream: Provides a single interface for all error messages needed for this exercice. Used for all system call errors.
ExecCD: Deals with "cd" command by managing errors. 

This code will only open 1 pipe at a time. Don't forget to close both file descriptors in parent and child processes. Makes it easy to avoid file desriptor leaks. 
