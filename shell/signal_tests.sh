#!/bin/bash

# Signal Testing Script for Minishell
# Tests both interactive and non-interactive signal handling

MINISHELL=./minishell
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=== MINISHELL SIGNAL TESTING ==="
echo "Testing signal behavior in interactive and non-interactive modes"
echo

# Test 1: Interactive Mode - SIGINT (Ctrl+C)
echo -e "${YELLOW}Test 1: Interactive Mode - SIGINT (Ctrl+C)${NC}"
echo "Starting minishell interactively and sending SIGINT..."

# Start minishell in background
timeout 5s $MINISHELL > /tmp/minishell_output.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
# Send SIGINT
kill -INT $MINISHELL_PID
sleep 1

# Check if minishell is still running (should be)
if kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ PASSED: Minishell survived SIGINT in interactive mode${NC}"
    kill -TERM $MINISHELL_PID 2>/dev/null
else
    echo -e "${RED}✗ FAILED: Minishell terminated on SIGINT in interactive mode${NC}"
fi

echo "Expected: Minishell should display ^C and continue running"
echo "Actual output:"
cat /tmp/minishell_output.txt
echo
echo "---"

# Test 2: Interactive Mode - SIGQUIT (Ctrl+\)
echo -e "${YELLOW}Test 2: Interactive Mode - SIGQUIT (Ctrl+\\)${NC}"
echo "Starting minishell interactively and sending SIGQUIT..."

timeout 5s $MINISHELL > /tmp/minishell_output2.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
# Send SIGQUIT
kill -QUIT $MINISHELL_PID
sleep 1

# Check if minishell is still running (should be)
if kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ PASSED: Minishell ignored SIGQUIT in interactive mode${NC}"
    kill -TERM $MINISHELL_PID 2>/dev/null
else
    echo -e "${RED}✗ FAILED: Minishell terminated on SIGQUIT in interactive mode${NC}"
fi

echo "Expected: Minishell should ignore SIGQUIT"
echo "Actual output:"
cat /tmp/minishell_output2.txt
echo
echo "---"

# Test 3: Interactive Mode - SIGINT with cat command
echo -e "${YELLOW}Test 3: Interactive Mode - SIGINT during cat command${NC}"
echo "Testing SIGINT during external command execution..."

(
    echo "cat"
    sleep 2
) | timeout 10s $MINISHELL > /tmp/minishell_output3.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
# Send SIGINT to interrupt cat
kill -INT $MINISHELL_PID
sleep 1

# Check if we get back to prompt
if grep -q "minishell" /tmp/minishell_output3.txt; then
    echo -e "${GREEN}✓ PASSED: SIGINT interrupted cat and returned to prompt${NC}"
else
    echo -e "${RED}✗ FAILED: SIGINT handling with external command${NC}"
fi

echo "Expected: ^C should interrupt cat and return to minishell prompt with exit status 130"
echo "Actual output:"
cat /tmp/minishell_output3.txt
echo
echo "---"

# Test 4: Non-Interactive Mode - SIGINT
echo -e "${YELLOW}Test 4: Non-Interactive Mode - SIGINT${NC}"
echo "Testing SIGINT in non-interactive mode (script mode)..."

# Create a test script
cat > /tmp/test_script.sh << 'EOF'
#!/bin/bash
echo "echo 'Before sleep'"
echo "sleep 10"
echo "echo 'After sleep - should not reach here'"
EOF

# Run script through minishell and send SIGINT
timeout 5s $MINISHELL < /tmp/test_script.sh > /tmp/minishell_output4.txt 2>&1 &
MINISHELL_PID=$!

sleep 2
kill -INT $MINISHELL_PID
sleep 1

# In non-interactive mode, SIGINT should terminate the shell
if ! kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ PASSED: SIGINT terminated minishell in non-interactive mode${NC}"
else
    echo -e "${RED}✗ FAILED: SIGINT did not terminate minishell in non-interactive mode${NC}"
    kill -TERM $MINISHELL_PID 2>/dev/null
fi

echo "Expected: Minishell should terminate on SIGINT in non-interactive mode"
echo "Actual output:"
cat /tmp/minishell_output4.txt
echo
echo "---"

# Test 5: Non-Interactive Mode - SIGQUIT
echo -e "${YELLOW}Test 5: Non-Interactive Mode - SIGQUIT${NC}"
echo "Testing SIGQUIT in non-interactive mode..."

echo "sleep 10" | timeout 5s $MINISHELL > /tmp/minishell_output5.txt 2>&1 &
MINISHELL_PID=$!

sleep 2
kill -QUIT $MINISHELL_PID
sleep 1

# In non-interactive mode, SIGQUIT should terminate the shell
if ! kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ PASSED: SIGQUIT terminated minishell in non-interactive mode${NC}"
else
    echo -e "${RED}✗ FAILED: SIGQUIT did not terminate minishell in non-interactive mode${NC}"
    kill -TERM $MINISHELL_PID 2>/dev/null
fi

echo "Expected: Minishell should terminate on SIGQUIT in non-interactive mode"
echo "Actual output:"
cat /tmp/minishell_output5.txt
echo
echo "---"

# Test 6: Heredoc Interruption
echo -e "${YELLOW}Test 6: Heredoc Interruption with SIGINT${NC}"
echo "Testing SIGINT during heredoc input..."

(
    echo "cat << EOF"
    sleep 3
) | timeout 10s $MINISHELL > /tmp/minishell_output6.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
kill -INT $MINISHELL_PID
sleep 1

echo "Expected: SIGINT should interrupt heredoc and return to prompt with exit status 130"
echo "Actual output:"
cat /tmp/minishell_output6.txt
if ! kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ Heredoc was interrupted${NC}"
else
    echo -e "${RED}✗ Heredoc interruption may have failed${NC}"
    kill -TERM $MINISHELL_PID 2>/dev/null
fi
echo
echo "---"

# Test 7: Exit Status After SIGINT
echo -e "${YELLOW}Test 7: Exit Status After Signal${NC}"
echo "Testing exit status after SIGINT..."

(
    echo "cat"
    sleep 2
    echo "echo \$?"
) | timeout 10s $MINISHELL > /tmp/minishell_output7.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
kill -INT $MINISHELL_PID
sleep 2

echo "Expected: Exit status should be 130 after SIGINT"
echo "Actual output:"
cat /tmp/minishell_output7.txt
echo
echo "---"

# Test 8: Child Process Signal Handling
echo -e "${YELLOW}Test 8: Child Process Signal Handling${NC}"
echo "Testing that child processes receive default signal behavior..."

(
    echo "sleep 5 &"
    echo "jobs"
    sleep 2
) | timeout 10s $MINISHELL > /tmp/minishell_output8.txt 2>&1 &
MINISHELL_PID=$!

sleep 1
# Send SIGINT - should interrupt the background sleep
kill -INT $MINISHELL_PID
sleep 1

echo "Expected: Background processes should handle signals independently"
echo "Actual output:"
cat /tmp/minishell_output8.txt
if ! kill -0 $MINISHELL_PID 2>/dev/null; then
    echo -e "${GREEN}✓ Child process signal handling test completed${NC}"
else
    kill -TERM $MINISHELL_PID 2>/dev/null
fi
echo

# Cleanup
rm -f /tmp/minishell_output*.txt /tmp/test_script.sh

echo "=== SIGNAL TESTING COMPLETED ==="
echo "Review the output above to verify signal handling behavior"
echo "Compare with bash behavior for reference"
