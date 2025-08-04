#!/bin/bash

echo "=== MINISHELL SIGNAL TEST CASES ==="
echo "Testing against the specific requirements"
echo ""

# Test Case 1: cat + CTRL+C
echo "Test Case 1: cat + CTRL+C"
echo "Expected: ^C followed by minishell> prompt, exit status 130"

{
    echo "cat"
    sleep 2
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 1
kill -INT $PID
wait $PID
echo ""

# Test Case 2: cat + CTRL+\
echo "Test Case 2: cat + CTRL+\\"  
echo "Expected: ^\\ followed by minishell> prompt, exit status 131"

{
    echo "cat"  
    sleep 2
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 1
kill -QUIT $PID
wait $PID
echo ""

# Test Case 3: cat + CTRL+D (EOF)
echo "Test Case 3: cat + CTRL+D (EOF)"
echo "Expected: Return to prompt, exit status 0"

{
    echo "cat"
    echo ""  # Simulate EOF
    echo "echo \$?"
    echo "exit"
} | ./minishell 2>&1
echo ""

# Test Case 4: Direct CTRL+C at prompt
echo "Test Case 4: CTRL+C at prompt"
echo "Expected: ^C followed by new prompt, exit status 130"

timeout 3s ./minishell 2>&1 &
PID=$!
sleep 1
kill -INT $PID
wait $PID
echo ""

# Test Case 5: Direct CTRL+D at prompt  
echo "Test Case 5: CTRL+D at prompt"
echo "Expected: Exit minishell, exit status 0"

echo "" | ./minishell 2>&1
echo "Exit status: $?"
echo ""

# Test Case 6: CTRL+\ at prompt
echo "Test Case 6: CTRL+\\ at prompt"
echo "Expected: Ignored (no output), exit status 0"

timeout 3s ./minishell 2>&1 &
PID=$!
sleep 1
kill -QUIT $PID
sleep 1
kill -TERM $PID 2>/dev/null
echo ""

# Test Case 7: Heredoc << end with CTRL+C
echo "Test Case 7: Heredoc with CTRL+C"
echo "Expected: ^C followed by prompt, exit status 130"

{
    echo "cat << end"
    sleep 2
    echo "echo \$?"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 1
kill -INT $PID
wait $PID
echo ""

# Test Case 8: Heredoc << end with CTRL+D
echo "Test Case 8: Heredoc with CTRL+D"  
echo "Expected: Warning message, exit status 0"

{
    echo "cat << end"
    echo ""  # EOF
    echo "echo \$?"
    echo "exit"
} | ./minishell 2>&1
echo ""

# Test Case 9: Heredoc << end with CTRL+\
echo "Test Case 9: Heredoc with CTRL+\\"
echo "Expected: No effect"

{
    echo "cat << end"
    sleep 1
    echo "test content"
    echo "end"
    echo "exit"
} | timeout 10s ./minishell 2>&1 &
PID=$!

sleep 0.5
kill -QUIT $PID
wait $PID
echo ""

echo "=== SPECIFIC TEST CASES COMPLETED ==="
