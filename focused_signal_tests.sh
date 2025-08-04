#!/bin/bash

echo "=== Testing Minishell Signal Behavior ==="

# Test 1: Interactive mode SIGINT
echo "Test 1: Interactive SIGINT (Ctrl+C)"
echo "Expected: Should show ^C and continue"
echo ""

# Start minishell and test SIGINT
{
    sleep 1
    pkill -INT minishell
    sleep 1 
    echo "exit" 
} | ./minishell

echo "✓ Interactive SIGINT test completed"
echo ""

# Test 2: Interactive mode SIGQUIT  
echo "Test 2: Interactive SIGQUIT (Ctrl+\\)"
echo "Expected: Should ignore signal and continue"
echo ""

{
    sleep 1
    pkill -QUIT minishell  
    sleep 1
    echo "exit"
} | ./minishell

echo "✓ Interactive SIGQUIT test completed"
echo ""

# Test 3: Non-interactive mode
echo "Test 3: Non-interactive mode"
echo "Expected: Should terminate on SIGINT"

# Create test script
echo "sleep 10" > /tmp/test_sleep.txt

# Run in non-interactive mode
./minishell < /tmp/test_sleep.txt &
PID=$!
sleep 1
kill -INT $PID 2>/dev/null

# Check if process terminated
sleep 1
if kill -0 $PID 2>/dev/null; then
    echo "✗ FAILED: Process still running after SIGINT"
    kill -TERM $PID 2>/dev/null
else
    echo "✓ PASSED: Process terminated on SIGINT in non-interactive mode"
fi

echo ""

# Test 4: Exit status after SIGINT
echo "Test 4: Exit status after SIGINT"

{
    echo "cat"
    sleep 2
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell &
PID=$!

sleep 1
kill -INT $PID 2>/dev/null
wait $PID
exit_status=$?

echo "Exit status after SIGINT: $exit_status"
if [ $exit_status -eq 130 ]; then
    echo "✓ PASSED: Correct exit status (130)"
else
    echo "✗ FAILED: Expected 130, got $exit_status"
fi

echo ""

# Cleanup
rm -f /tmp/test_sleep.txt

echo "=== Signal Tests Completed ==="
echo ""
echo "Summary:"
echo "- Interactive mode should handle SIGINT with ^C and continue"
echo "- Interactive mode should ignore SIGQUIT"  
echo "- Non-interactive mode should terminate on SIGINT/SIGQUIT"
echo "- Exit status after SIGINT should be 130"
