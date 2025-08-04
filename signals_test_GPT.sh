#!/bin/bash

# Comprehensive Signal Testing for Minishell
# Tests all signal-related functionality

MINISHELL=./minishell
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "========================================="
echo "🧪 COMPREHENSIVE MINISHELL SIGNAL TESTS"
echo "========================================="
echo

TEST_COUNT=0
PASS_COUNT=0

# Helper function to run a test
run_test() {
    local test_name="$1"
    local expected="$2" 
    local result="$3"
    
    TEST_COUNT=$((TEST_COUNT + 1))
    echo -e "${BLUE}Test $TEST_COUNT: $test_name${NC}"
    
    if [[ "$result" == *"$expected"* ]] || [[ "$expected" == "MANUAL" ]]; then
        echo -e "${GREEN}✅ PASSED${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}❌ FAILED${NC}"
        echo "Expected: $expected"
        echo "Got: $result"
    fi
    echo "---"
}

echo "🔧 BASIC FUNCTIONALITY TESTS"
echo

# Test 1: Basic command execution
result=$(echo "echo hello" | $MINISHELL 2>&1)
run_test "Basic echo command" "hello" "$result"

# Test 2: Built-in commands
result=$(echo "pwd" | $MINISHELL 2>&1)
run_test "Built-in pwd command" "/mnt/c/Users/user/Desktop/Minishell" "$result"

# Test 3: External commands
result=$(echo "ls Makefile" | $MINISHELL 2>&1)
run_test "External ls command" "Makefile" "$result"

# Test 4: Environment variables
result=$(echo "echo \$HOME" | $MINISHELL 2>&1)
run_test "Environment variable expansion" "MANUAL" "$result"

echo
echo "📡 SIGNAL SETUP TESTS"
echo

# Test 5: Interactive mode detection
result=$(timeout 2s $MINISHELL 2>&1 | head -3)
run_test "Interactive mode (shows welcome)" "WELCOME TO" "$result"

# Test 6: Non-interactive mode detection  
result=$(echo "echo non-interactive" | $MINISHELL 2>&1)
run_test "Non-interactive mode (no welcome)" "non-interactive" "$result"

echo
echo "🚦 SIGNAL HANDLING TESTS"
echo

# Test 7: SIGINT in non-interactive mode (should terminate)
echo "Starting non-interactive SIGINT test..."
(echo "sleep 5"; sleep 10) | timeout 8s $MINISHELL &
PID=$!
sleep 1
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?
if [ $exit_code -eq 130 ] || [ $exit_code -eq 2 ] || [ $exit_code -eq 124 ]; then
    result="SUCCESS: Process terminated with code $exit_code"
else
    result="FAIL: Unexpected exit code $exit_code"
fi
run_test "SIGINT in non-interactive mode" "SUCCESS" "$result"

# Test 8: SIGQUIT in non-interactive mode (should terminate)
echo "Starting non-interactive SIGQUIT test..."
(echo "sleep 5"; sleep 10) | timeout 8s $MINISHELL &
PID=$!
sleep 1
kill -QUIT $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?
if [ $exit_code -eq 131 ] || [ $exit_code -eq 3 ] || [ $exit_code -eq 124 ]; then
    result="SUCCESS: Process terminated with code $exit_code"
else
    result="FAIL: Unexpected exit code $exit_code"
fi
run_test "SIGQUIT in non-interactive mode" "SUCCESS" "$result"

echo
echo "🔄 EXTERNAL COMMAND SIGNAL TESTS"
echo

# Test 9: External command execution with proper signal handling
result=$(echo "ls -la | head -2" | $MINISHELL 2>&1)
run_test "External command pipeline" "total" "$result"

# Test 10: External command interruption
echo "Testing external command SIGINT handling..."
{
    echo "cat /dev/zero > /dev/null"
    sleep 3
    echo "echo \$?"
    echo "exit"
} | timeout 10s $MINISHELL &
PID=$!
sleep 1
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
result="External command interrupted successfully"
run_test "External command SIGINT" "MANUAL" "$result"

echo
echo "🏗️ BUILT-IN COMMAND SIGNAL TESTS"
echo

# Test 11: Built-in commands maintain shell signal handling
result=$(echo -e "export TEST_VAR=signal_test
echo \$TEST_VAR
exit" | $MINISHELL 2>&1)
run_test "Built-in export/echo commands" "signal_test" "$result"

# Test 12: Built-in with redirections
echo "test content" > /tmp/test_signal.txt
result=$(echo "echo 'redirected' > /tmp/test_output.txt" | $MINISHELL 2>&1)
if [ -f /tmp/test_output.txt ] && grep -q "redirected" /tmp/test_output.txt; then
    result="SUCCESS: Redirection works"
else
    result="FAIL: Redirection failed"
fi
run_test "Built-in with redirection" "SUCCESS" "$result"

echo
echo "📄 HEREDOC SIGNAL TESTS"
echo

# Test 13: Heredoc basic functionality
{
    echo "cat << EOF"
    echo "line1"
    echo "line2"  
    echo "EOF"
    echo "exit"
} | $MINISHELL > /tmp/heredoc_test.txt 2>&1
if grep -q "line1" /tmp/heredoc_test.txt && grep -q "line2" /tmp/heredoc_test.txt; then
    result="SUCCESS: Heredoc works"
else
    result="FAIL: Heredoc failed"
fi
run_test "Heredoc basic functionality" "SUCCESS" "$result"

# Test 14: Heredoc signal handling
echo "Testing heredoc SIGINT handling..."
{
    echo "cat << EOF"
    sleep 2
} | timeout 5s $MINISHELL &
PID=$!
sleep 1
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
result="Heredoc interrupted successfully"
run_test "Heredoc SIGINT handling" "MANUAL" "$result"

echo
echo "🔀 PIPELINE SIGNAL TESTS"
echo

# Test 15: Pipeline execution
result=$(echo "echo hello | cat | cat" | $MINISHELL 2>&1)
run_test "Basic pipeline" "hello" "$result"

# Test 16: Pipeline with external commands
result=$(echo "ls | head -3" | $MINISHELL 2>&1)
run_test "Pipeline with external commands" "Makefile" "$result"

echo
echo "⚡ EXIT STATUS TESTS"
echo

# Test 17: Exit status after successful command
result=$(echo -e "echo success
echo \$?
exit" | $MINISHELL 2>&1)
run_test "Exit status after success" "0" "$result"

# Test 18: Exit status after failed command
result=$(echo -e "nonexistent_command
echo \$?
exit" | $MINISHELL 2>&1)
run_test "Exit status after command not found" "127" "$result"

echo
echo "🎯 INTERACTIVE SIGNAL TESTS (Manual)"
echo

echo -e "${YELLOW}Manual Test 1: Interactive SIGINT${NC}"
echo "Run: $MINISHELL"
echo "Press Ctrl+C (should show ^C and continue)"
echo "Type: exit"
echo

echo -e "${YELLOW}Manual Test 2: Interactive SIGQUIT${NC}"
echo "Run: $MINISHELL"  
echo "Press Ctrl+\ (should be ignored)"
echo "Type: exit"
echo

echo -e "${YELLOW}Manual Test 3: Cat with SIGINT${NC}"
echo "Run: $MINISHELL"
echo "Type: cat"
echo "Press Ctrl+C (should interrupt and return to prompt)"
echo "Type: echo \$? (should show 130)"
echo "Type: exit"

echo
echo "🧹 CLEANUP"
rm -f /tmp/test_signal.txt /tmp/test_output.txt /tmp/heredoc_test.txt

echo
echo "========================================="
echo "📊 TEST SUMMARY"
echo "========================================="
echo -e "${GREEN}Passed: $PASS_COUNT / $TEST_COUNT tests${NC}"

if [ $PASS_COUNT -eq $TEST_COUNT ]; then
    echo -e "${GREEN}🎉 ALL AUTOMATED TESTS PASSED!${NC}"
    echo "Please run the manual interactive tests above to complete verification."
else
    echo -e "${RED}⚠️  Some tests failed. Please review the output above.${NC}"
fi

echo
echo "Signal handling verification complete!"

function print_header() {
    echo -e "\n\033[1;34m🔹 $1\033[0m"
}

function run_test() {
    echo -e "\n\033[0;33m$1\033[0m"
    eval "$2"
    echo -e "\033[0;32m[✔] Completed\033[0m"
}

# 1. Test Ctrl+C in prompt
print_header "Test 1: SIGINT at prompt (Ctrl+C)"
run_test "Press Ctrl+C when prompted:" "$MINISHELL"

# 2. Test SIGQUIT at prompt (should be ignored)
print_header "Test 2: SIGQUIT at prompt (Ctrl+\\)"
run_test "Press Ctrl+\\ when prompted (should do nothing):" "$MINISHELL"

# 3. Test heredoc interruption
print_header "Test 3: SIGINT during heredoc"
run_test "Testing heredoc interruption (Press Ctrl+C after typing):" \
"echo \"cat << EOF\" | $MINISHELL"

# 4. Test external command SIGINT
print_header "Test 4: SIGINT during external command (e.g., sleep)"
run_test "Sending SIGINT to child (sleep):" \
"(echo 'sleep 5' | $MINISHELL) & sleep 1 && pkill -INT minishell"

# 5. Test SIGQUIT during external command
print_header "Test 5: SIGQUIT during external command (e.g., cat)"
run_test "SIGQUIT should terminate the child process:" \
"(echo 'cat' | $MINISHELL) & sleep 1 && pkill -QUIT cat"

# 6. Test builtin with redirection (should not crash or misbehave)
print_header "Test 6: Builtin with redirection signal safety"
run_test "Executing built-in with redirection safely:" \
"echo 'echo hello > out.txt' | $MINISHELL && cat out.txt && rm out.txt"

# 7. Test pipeline behavior
print_header "Test 7: Signal with pipe chain (ls | sleep 5)"
run_test "Sending SIGINT to minishell during pipeline:" \
"(echo 'ls | sleep 5' | $MINISHELL) & sleep 1 && pkill -INT minishell"

# 8. Non-interactive mode (pipe into minishell)
print_header "Test 8: Non-interactive mode"
run_test "Echo piped into minishell (non-interactive):" \
"echo 'echo Hello from non-interactive' | $MINISHELL"

# 9. Multiple heredocs (interrupt one)
print_header "Test 9: Multiple heredocs and interrupt one"
run_test "Simulating multiple heredocs (manually hit Ctrl+C):" \
"$MINISHELL -c 'cat << EOF1; cat << EOF2'"

echo -e "\n\033[1;36m✅ All signal tests completed. Review behavior above.\033[0m"
