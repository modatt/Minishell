#!/bin/bash

# Configuration
MINISHELL_PATH="./minishell"
TEMP_DIR="minishell_signal_test_results"

# Ensure temporary directory and files are cleaned up on exit
trap 'rm -rf "$TEMP_DIR"; exit' EXIT INT TERM

# Function to run a signal test
# Args:
#   $1: Test Name
#   $2: Command to pipe into minishell (e.g., "sleep 2")
#   $3: Signal to send (e.g., "INT", "QUIT")
#   $4: Expected Minishell Exit Status (e.g., 130, 131, 0 for successful exit)
#   $5: Expected Output Pattern (regex or exact string, can be empty)
test_signal_behavior() {
    local test_name="$1"
    local command_to_pipe="$2"
    local signal_name="$3"
    local expected_status="$4"
    local expected_output_pattern="$5"

    local output_file="$TEMP_DIR/output_${test_name// /_}.txt"
    local status_file="$TEMP_DIR/status_${test_name// /_}.txt"

    echo "--- Test: $test_name (Signal: $signal_name) ---"
    echo "Command: '$command_to_pipe'"

    # Run minishell in the background, piping input to it
    # We use 'bash -c' to ensure the command is executed in a subshell,
    # and timeout to prevent hangs if minishell doesn't exit.
    (echo "$command_to_pipe" | timeout 5s "$MINISHELL_PATH" > "$output_file" 2>&1) &
    local minishell_pid=$! # Get PID of the background minishell process

    # Give minishell a moment to start and execute the command
    sleep 0.1

    # Send the signal to minishell
    kill "-$signal_name" "$minishell_pid" >/dev/null 2>&1

    # Wait for minishell to terminate and capture its exit status
    wait "$minishell_pid"
    local actual_status=$?
    echo "$actual_status" > "$status_file"

    local status="FAIL"
    local output_match="NO"

    # Compare exit status
    if [ "$actual_status" -eq "$expected_status" ]; then
        status="PASS"
    fi

    # Compare output
    if [ -z "$expected_output_pattern" ]; then
        # If expected output is empty, check if actual output file is empty
        if [ ! -s "$output_file" ]; then # -s checks if file exists and is not empty
            output_match="YES"
        else
            output_match="NO (Expected empty, got: $(cat "$output_file"))"
        fi
    else
        # Check if output matches pattern (using grep for regex)
        if grep -qE "$expected_output_pattern" "$output_file"; then
            output_match="YES"
        else
            output_match="NO (Pattern: '$expected_output_pattern' not found in output)"
        fi
    fi

    echo "Result: Status $status (Expected: $expected_status, Actual: $actual_status)"
    echo "Output Match: $output_match"

    if [ "$status" == "FAIL" ] || [ "$output_match" == "NO" ]; then
        echo "--- Minishell Output ---"
        cat "$output_file"
        echo "--------------------------------------------------"
        return 1 # Indicate test failure
    else
        echo "--------------------------------------------------"
        return 0 # Indicate test success
    fi
}

# --- Setup ---
mkdir -p "$TEMP_DIR"
echo "Starting automated signal tests..."

# --- Test Cases ---
total_tests=0
failed_tests=0

# Test 1: Non-interactive SIGINT (minishell should terminate)
total_tests=$((total_tests + 1))
test_signal_behavior "Non-interactive SIGINT (shell)" "sleep 2" "INT" 130 "" || failed_tests=$((failed_tests + 1))

# Test 2: Non-interactive SIGQUIT (minishell should terminate)
total_tests=$((total_tests + 1))
test_signal_behavior "Non-interactive SIGQUIT (shell)" "sleep 2" "QUIT" 131 "" || failed_tests=$((failed_tests + 1))

# Test 3: Child process SIGINT (minishell should report 130 for child)
# This test is tricky: we send SIGINT to minishell, but minishell's child should handle it.
# Minishell itself should then report the child's exit status.
# The 'kill' command is sent to minishell's PID, not the child's.
# Minishell's waitpid should capture the child's signal termination.
# The output should be empty because minishell is non-interactive.
total_tests=$((total_tests + 1))
test_signal_behavior "Child process SIGINT (via shell)" "sleep 2" "INT" 130 "" || failed_tests=$((failed_tests + 1))

# Test 4: Child process SIGQUIT (minishell should report 131 for child)
total_tests=$((total_tests + 1))
test_signal_behavior "Child process SIGQUIT (via shell)" "sleep 2" "QUIT" 131 "Quit \(core dumped\)" || failed_tests=$((failed_tests + 1))


# Test 5: Non-interactive Ctrl+D (EOF) - no signal, just normal exit
# This tests the main_loop's EOF handling, not signal handling.
total_tests=$((total_tests + 1))
test_signal_behavior "Non-interactive EOF (Ctrl+D)" "" "" 0 "" || failed_tests=$((failed_tests + 1))


# --- Summary ---
echo "=================================================="
echo "Signal Test Summary:"
echo "Total tests: $total_tests"
echo "Failed tests: $failed_tests"
echo "=================================================="

if [ "$failed_tests" -gt 0 ]; then
    echo "Some signal tests FAILED. Check '$TEMP_DIR' for details."
    exit 1
else
    echo "All signal tests PASSED!"
    exit 0
fi
