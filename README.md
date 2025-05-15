# Minishell
 A small version of a shell

 ## Functions:

    >> Readline()
take a string and 

   >> rl_on_new_line();
Simple meaning:
Tells readline “Hey, we are now starting a new line.”

Why use it?
If something (like a signal or print message) messes up the line you're typing, this resets it — like saying “ok, fresh new line”.

Example use:
If the user pressed Ctrl+C, and you want to reset their input line cleanly.


   >> rl_redisplay();
Simple meaning:
Redraws what the user is typing.
It refreshes the line on the screen.

Why use it?
After you clean/reset the line or print something, you need to show the current input again.

Example use:

rl_on_new_line();
rl_redisplay();

This combination:
✔ starts a new line
✔ and redraws the user’s input line correctly.

   >> access()
access() is used to check if a file exists and if you have permission to access it (like execute it).

#include <unistd.h>

int access(const char *pathname, int mode);

pathname: path to the file (e.g., /bin/ls, ./my_program)

mode: what you want to check. Common options:

F_OK → Check if file exists

X_OK → Check if file is executable

R_OK → Check if file is readable

W_OK → Check if file is writable