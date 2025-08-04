#!/bin/bash

# Specific tests for the failed signal handling cases
# Focuses on the 3 failing tests from the comprehensive analysis

MINISHELL=./minishell
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== FOCUSED SIGNAL PROBLEM TESTING ===${NC}"
echo "Testing the specific issues that failed in comprehensive analysis"
echo ""

# Test 1: Builtin Command Signal Handling Issue
echo -e "${YELLOW}Test 1: Builtin Command Signal Handling${NC}"
echo "Testing export command under signal stress..."

# Create a more controlled test for export
cat << 'EOF' > /tmp/export_test.sh
export TEST_VAR=test_value
echo "Exported: $TEST_VAR"
sleep 2
echo "After sleep: $TEST_VAR"
EOF

{
    cat /tmp/export_test.sh
    echo "exit"
} | timeout 5s $MINISHELL &
PID=$!
sleep 1
echo "Sending SIGINT to minishell..."
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?

echo "Exit code: $exit_code"
if [ $exit_code -eq 130 ]; then
    echo -e "${GREEN}✓ FIXED: Builtin command signal handling${NC}"
else
    echo -e "${RED}✗ STILL FAILING: Builtin command signal handling (exit code: $exit_code)${NC}"
fi
rm -f /tmp/export_test.sh
echo ""

# Test 2: External Command Signal Forwarding Issue
echo -e "${YELLOW}Test 2: External Command Signal Forwarding${NC}"
echo "Testing signal forwarding to external sleep command..."

{
    echo "sleep 5"
    echo "echo 'This should not appear'"
    echo "echo \$?"
    echo "exit"
} | timeout 8s $MINISHELL &
PID=$!
sleep 2
echo "Sending SIGINT to minishell with external command..."
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?

echo "Exit code: $exit_code"
if [ $exit_code -eq 130 ]; then
    echo -e "${GREEN}✓ FIXED: External command signal forwarding${NC}"
else
    echo -e "${RED}✗ STILL FAILING: External command signal forwarding (exit code: $exit_code)${NC}"
fi
echo ""

echo -e "${YELLOW}=== FOCUSED TEST SUMMARY ===${NC}"
echo "1. Builtin command signal handling - Tested"
echo "2. External command signal forwarding - Tested" 
echo ""

echo -e "${YELLOW}=== NEXT STEPS ===${NC}"
echo "1. Replace old signal files with unified implementation"
echo "2. Update Makefile to use signals_unified.c"
echo "3. Remove conflicting signal handler files"
echo "4. Test thoroughly with both interactive and non-interactive modes"
echo "5. Run valgrind tests for memory leak verification"
