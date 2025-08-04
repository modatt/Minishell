#!/bin/bash

echo "=== Debugging Minishell Step by Step ==="

MINISHELL=./minishell

echo "Test 1: Simple echo command"
echo "echo hello" | timeout 2s $MINISHELL
echo "Exit code: $?"
echo

echo "Test 2: pwd command"  
echo "pwd" | timeout 2s $MINISHELL
echo "Exit code: $?"
echo

echo "Test 3: env command"
echo "env" | timeout 2s $MINISHELL | head -5
echo "Exit code: $?"
echo

echo "Test 4: Multiple commands"
printf "echo first\necho second\nexit\n" | timeout 2s $MINISHELL
echo "Exit code: $?"
echo

echo "=== Debug completed ==="
