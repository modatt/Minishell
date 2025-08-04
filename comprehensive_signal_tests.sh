#!/bin/bash

echo "=== MINISHELL SIGNAL TESTING ==="
echo ""

# Test 1: Basic functionality 
echo "Test 1: Basic functionality"
echo "Testing echo command:"
result=$(echo "echo 'Hello World'" | ./minishell 2>&1)
echo "$result"
echo ""

# Test 2: Interactive mode detection
echo "Test 2: Interactive vs Non-interactive mode"
echo "Interactive mode (with TTY simulation):"
result=$(timeout 2s ./minishell 2>&1 | head -5)
echo "$result"
echo ""

echo "Non-interactive mode (pipe input):"
result=$(echo "echo 'Non-interactive test'" | ./minishell 2>&1)
echo "$result"
echo ""

# Test 3: SIGINT in non-interactive mode
echo "Test 3: SIGINT in non-interactive mode"
echo "Starting minishell with 'sleep 10', then sending SIGINT..."

(echo "sleep 10"; sleep 20) | timeout 10s ./minishell 2>&1 &
PID=$!
sleep 2
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?

if [ $exit_code -eq 130 ] || [ $exit_code -eq 2 ]; then
    echo "✓ PASSED: Minishell terminated with appropriate exit code ($exit_code)"
else
    echo "✗ Result: Exit code was $exit_code"
fi
echo ""

# Test 4: SIGQUIT in non-interactive mode  
echo "Test 4: SIGQUIT in non-interactive mode"
echo "Starting minishell with 'sleep 10', then sending SIGQUIT..."

(echo "sleep 10"; sleep 20) | timeout 10s ./minishell 2>&1 &
PID=$!
sleep 2
kill -QUIT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?

if [ $exit_code -eq 131 ] || [ $exit_code -eq 3 ]; then
    echo "✓ PASSED: Minishell terminated with appropriate exit code ($exit_code)"
else
    echo "✗ Result: Exit code was $exit_code"
fi
echo ""

# Test 5: External command signal handling
echo "Test 5: External command signal handling"
echo "Testing cat command interrupted by SIGINT:"

{
    echo "cat"
    sleep 3
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 1
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null

echo "Expected: cat should be interrupted and exit status should be 130"
echo ""

# Test 6: Built-in vs external command signal handling
echo "Test 6: Built-in command during signal"
result=$(echo -e "export TEST=value\necho \$TEST\nexit" | ./minishell 2>&1)
echo "Built-in commands test:"
echo "$result"
echo ""

# Test 7: Multiple commands with signals
echo "Test 7: Command sequence with interruption"
{
    echo "echo 'Before command'"
    echo "cat"  # This will wait for input
    sleep 2
    echo "echo 'After interrupt (should show exit status)'"
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 1
kill -INT $PID 2>/dev/null
result=$(wait $PID 2>&1)
echo "Result of interrupted command sequence:"
echo "$result"
echo ""

echo "=== SIGNAL TESTING COMPLETED ==="
echo ""
echo "Manual Tests to Perform:"
echo "1. Run: ./minishell"
echo "   Then press Ctrl+C (should show ^C and continue)"
echo "   Then press Ctrl+\\ (should be ignored)"
echo "   Type: exit"
echo ""
echo "2. Run: echo 'cat' | ./minishell"
echo "   Then quickly press Ctrl+C"
echo "   Should interrupt and show exit status 130"
