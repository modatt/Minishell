#!/bin/bash

# Simple Signal Tests for Minishell
MINISHELL=./minishell

echo "=== Simple Signal Tests ==="

# Test 1: Check if minishell detects interactive vs non-interactive mode
echo "Test 1: Mode Detection"
echo "Interactive test (should show welcome message):"
echo "exit" | $MINISHELL
echo ""

echo "Non-interactive test (should not show welcome message):"
echo "exit" > /tmp/test_input.txt
$MINISHELL < /tmp/test_input.txt
echo ""

# Test 2: Manual interactive signal test
echo "Test 2: Manual Interactive Signal Test"
echo "Starting minishell... Press Ctrl+C then type 'exit' to quit"
echo "You should see ^C and return to prompt"
$MINISHELL
echo ""

# Test 3: Test cat with SIGINT
echo "Test 3: Testing cat command with SIGINT"
echo "This will start cat, wait 2 seconds, send SIGINT, then exit"
(
    echo "cat"
    sleep 2
    echo "exit"
) | timeout 10s $MINISHELL &
PID=$!
sleep 1
kill -INT $PID
wait $PID
echo "Exit status: $?"
echo ""

# Test 4: Test exit status after interrupted command
echo "Test 4: Exit status after SIGINT"
(
    echo "cat"
    sleep 1
    echo "echo \$?"
    echo "exit"
) | timeout 10s $MINISHELL &
PID=$!
sleep 0.5
kill -INT $PID
sleep 1
kill -TERM $PID 2>/dev/null
echo ""

# Test 5: Non-interactive mode with signals
echo "Test 5: Non-interactive with SIGINT (should terminate)"
echo "sleep 5" | $MINISHELL &
PID=$!
sleep 1
kill -INT $PID
if kill -0 $PID 2>/dev/null; then
    echo "FAILED: Process still running"
    kill -TERM $PID
else
    echo "PASSED: Process terminated by SIGINT"
fi
echo ""

# Cleanup
rm -f /tmp/test_input.txt

echo "=== Tests Completed ==="
