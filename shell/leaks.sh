#!/bin/bash

MINISHELL=./minishell
LOG=valgrind.log

echo "===== Minishell Stress Test ====="
rm -f $LOG

# Function to run a single test
run_test() {
    echo -e "\n[Test] $1"
    echo "$2" | valgrind --leak-check=full --track-fds=yes \
        --quiet --log-file=$LOG --error-exitcode=42 $MINISHELL > /dev/null
    if [ $? -eq 42 ]; then
        echo "  ❌ Valgrind errors detected! Check $LOG"
    else
        echo "  ✅ No leaks / unclosed FDs detected."
    fi
}

# ----------------------
# Basic Commands
# ----------------------
run_test "Simple command" "ls"
run_test "Multiple arguments" "echo Hello World"
run_test "Environment variable" "echo \$HOME"

# ----------------------
# Pipes
# ----------------------
run_test "Single pipe" "ls | wc -l"
run_test "Multiple pipes" "cat /etc/passwd | grep root | wc -l"

# ----------------------
# Redirections
# ----------------------
run_test "Output redirection" "echo Hello > out.txt"
run_test "Append redirection" "echo Again >> out.txt"
run_test "Input redirection" "cat < out.txt"

# ----------------------
# Mixed pipes + redirection
# ----------------------
run_test "Pipe + redirect" "ls | grep .c > files.txt"

# ----------------------
# Built-ins
# ----------------------
run_test "cd builtin" "cd / && pwd"
run_test "export builtin" "export TEST=123 && echo \$TEST"
run_test "unset builtin" "export TEMP=456 && unset TEMP && echo \$TEMP"
run_test "exit builtin" "exit"

# ----------------------
# Stress Tests
# ----------------------
run_test "Many pipes" "yes | head -n 1000 | grep yes | wc -l"
run_test "Long command" "$(yes echo | head -n 50 | tr '\n' ' ')"

# ----------------------
# Cleanup
# ----------------------
rm -f out.txt files.txt

echo -e "\n===== All Tests Done ====="
