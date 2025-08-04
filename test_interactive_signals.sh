#!/bin/bash

# Test interactive signal handling using script to create a pseudo-TTY
MINISHELL=./minishell

echo "=== Testing Interactive Signal Handling ==="

# Test 1: Use script to create a pseudo-TTY for true interactive mode
echo "Test 1: Interactive SIGINT handling"
echo "Creating pseudo-TTY environment..."

# Create a test script that will send commands to minishell
cat << 'EOF' > /tmp/test_commands.txt
pwd
echo "Before signal test"
exit
EOF

# Use script to create a pseudo-TTY
timeout 10s script -q -c "$MINISHELL" /tmp/minishell_interactive.log < /tmp/test_commands.txt &
SCRIPT_PID=$!

sleep 2

# Find the minishell process (child of script)
MINISHELL_PID=$(pgrep -P $SCRIPT_PID minishell)

if [ -n "$MINISHELL_PID" ]; then
    echo "Found minishell PID: $MINISHELL_PID"
    echo "Sending SIGINT..."
    kill -INT $MINISHELL_PID
    sleep 1
    
    # Check if minishell is still running
    if kill -0 $MINISHELL_PID 2>/dev/null; then
        echo "✓ SIGINT test passed - minishell survived"
        kill -TERM $SCRIPT_PID 2>/dev/null
    else
        echo "✗ SIGINT test failed - minishell terminated"
    fi
else
    echo "✗ Could not find minishell process"
    kill -TERM $SCRIPT_PID 2>/dev/null
fi

# Wait for background processes to clean up
wait 2>/dev/null

echo "Interactive session output:"
cat /tmp/minishell_interactive.log 2>/dev/null

# Clean up
rm -f /tmp/test_commands.txt /tmp/minishell_interactive.log

echo "=== Test completed ==="
