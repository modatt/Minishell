#!/bin/bash

# Configuration: Path to your minishell executable
MINISHELL_PATH="./minishell"

# Temporary directory for storing test results
TEMP_DIR="minishell_test_results"

# --- Helper Functions ---

# Function to run a command in bash and capture its stdout/stderr and exit status
# Arguments:
#   $1: The command string to execute
#   $2: File path to store bash's stdout/stderr
#   $3: File path to store bash's exit status
run_bash_test() {
    local cmd="$1"
    local output_file="$2"
    local exit_status_file="$3"
    echo "Running in bash: $cmd"
    # Execute the command in a subshell to isolate environment changes
    bash -c "$cmd" > "$output_file" 2>&1
    echo $? > "$exit_status_file"
}

# Function to run a command in minishell and capture its stdout/stderr and exit status
# Arguments:
#   $1: The command string to execute
#   $2: File path to store minishell's stdout/stderr
#   $3: File path to store minishell's exit status
run_minishell_test() {
    local cmd="$1"
    local output_file="$2"
    local exit_status_file="$3"
    echo "Running in minishell: $cmd"
    # Execute minishell by piping the command to its stdin.
    # This assumes minishell reads commands from stdin and executes them.
    # If your minishell has a different execution method (e.g., a -c flag),
    # you will need to modify this line.
    echo "$cmd" | "$MINISHELL_PATH" > "$output_file" 2>&1
    echo $? > "$exit_status_file"
}

# Function to compare the results of bash and minishell for a given test case
# Arguments:
#   $1: Name of the test case
#   $2: File path to bash's captured output
#   $3: File path to minishell's captured output
#   $4: File path to bash's captured exit status
#   $5: File path to minishell's captured exit status
compare_results() {
    local test_name="$1"
    local bash_output="$2"
    local minishell_output="$3"
    local bash_exit="$4"
    local minishell_exit="$5"

    echo -e "\n--- Test Case: $test_name ---"
    local status="PASS"

    # Compare stdout/stderr using diff
    if diff -q "$bash_output" "$minishell_output" > /dev/null; then
        echo "Output: MATCH"
    else
        echo "Output: MISMATCH"
        echo "--- Bash Output ---"
        cat "$bash_output"
        echo "--- Minishell Output ---"
        cat "$minishell_output"
        status="FAIL"
    fi

    # Compare exit status
    local b_exit=$(cat "$bash_exit")
    local m_exit=$(cat "$minishell_exit")
    if [ "$b_exit" -eq "$m_exit" ]; then
        echo "Exit Status: MATCH ($b_exit)"
    else
        echo "Exit Status: MISMATCH (Bash: $b_exit, Minishell: $m_exit)"
        status="FAIL"
    fi

    echo -e "Result: $status\n"
    if [ "$status" == "FAIL" ]; then
        return 1 # Indicate failure
    fi
    return 0 # Indicate success
}

# --- Setup and Cleanup Functions ---

# Sets up the test environment by creating a temporary directory
setup_test_environment() {
    rm -rf "$TEMP_DIR" # Remove any previous test results
    mkdir -p "$TEMP_DIR" # Create the directory
    echo "Test environment set up in '$TEMP_DIR'"
}

# Cleans up the test environment by removing the temporary directory
cleanup_test_environment() {
    echo "Cleaning up test environment..."
    rm -rf "$TEMP_DIR"
    echo "Cleanup complete."
}

# --- Main Test Execution Function ---
run_tests() {
    setup_test_environment

    local test_count=0
    local fail_count=0

    echo "===================================================================="
    echo "Starting export tests..."
    echo "===================================================================="

    # Test 1: Simple export of a new variable
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_1=hello; echo \$VAR_TEST_1" "$TEMP_DIR/bash_out_1.txt" "$TEMP_DIR/bash_exit_1.txt"
    run_minishell_test "export VAR_TEST_1=hello; echo \$VAR_TEST_1" "$TEMP_DIR/minishell_out_1.txt" "$TEMP_DIR/minishell_exit_1.txt"
    compare_results "T$test_count: Simple export VAR_TEST_1=hello" "$TEMP_DIR/bash_out_1.txt" "$TEMP_DIR/minishell_out_1.txt" "$TEMP_DIR/bash_exit_1.txt" "$TEMP_DIR/minishell_exit_1.txt" || fail_count=$((fail_count + 1))

    # Test 2: Exporting an existing non-exported variable
    test_count=$((test_count + 1))
    run_bash_test "VAR_TEST_2=world; export VAR_TEST_2; echo \$VAR_TEST_2" "$TEMP_DIR/bash_out_2.txt" "$TEMP_DIR/bash_exit_2.txt"
    run_minishell_test "VAR_TEST_2=world; export VAR_TEST_2; echo \$VAR_TEST_2" "$TEMP_DIR/minishell_out_2.txt" "$TEMP_DIR/minishell_exit_2.txt"
    compare_results "T$test_count: Export existing non-exported VAR_TEST_2" "$TEMP_DIR/bash_out_2.txt" "$TEMP_DIR/minishell_out_2.txt" "$TEMP_DIR/bash_exit_2.txt" "$TEMP_DIR/minishell_exit_2.txt" || fail_count=$((fail_count + 1))

    # Test 3: Exporting an empty variable
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_3=\"\"; echo \"\$VAR_TEST_3\"" "$TEMP_DIR/bash_out_3.txt" "$TEMP_DIR/bash_exit_3.txt"
    run_minishell_test "export VAR_TEST_3=\"\"; echo \"\$VAR_TEST_3\"" "$TEMP_DIR/minishell_out_3.txt" "$TEMP_DIR/minishell_exit_3.txt"
    compare_results "T$test_count: Export empty VAR_TEST_3" "$TEMP_DIR/bash_out_3.txt" "$TEMP_DIR/minishell_out_3.txt" "$TEMP_DIR/bash_exit_3.txt" "$TEMP_DIR/minishell_exit_3.txt" || fail_count=$((fail_count + 1))

    # Test 4: Exporting with spaces and quotes
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_4=\"value with spaces\"; echo \"\$VAR_TEST_4\"" "$TEMP_DIR/bash_out_4.txt" "$TEMP_DIR/bash_exit_4.txt"
    run_minishell_test "export VAR_TEST_4=\"value with spaces\"; echo \"\$VAR_TEST_4\"" "$TEMP_DIR/minishell_out_4.txt" "$TEMP_DIR/minishell_exit_4.txt"
    compare_results "T$test_count: Export VAR_TEST_4 with spaces" "$TEMP_DIR/bash_out_4.txt" "$TEMP_DIR/minishell_out_4.txt" "$TEMP_DIR/bash_exit_4.txt" "$TEMP_DIR/minishell_exit_4.txt" || fail_count=$((fail_count + 1))

    # Test 5: Exporting using another variable's value
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_5_ORIGINAL=original_value; export VAR_TEST_5=\$VAR_TEST_5_ORIGINAL; echo \"\$VAR_TEST_5\"" "$TEMP_DIR/bash_out_5.txt" "$TEMP_DIR/bash_exit_5.txt"
    run_minishell_test "export VAR_TEST_5_ORIGINAL=original_value; export VAR_TEST_5=\$VAR_TEST_5_ORIGINAL; echo \"\$VAR_TEST_5\"" "$TEMP_DIR/minishell_out_5.txt" "$TEMP_DIR/minishell_exit_5.txt"
    compare_results "T$test_count: Export VAR_TEST_5 using another variable" "$TEMP_DIR/bash_out_5.txt" "$TEMP_DIR/minishell_out_5.txt" "$TEMP_DIR/bash_exit_5.txt" "$TEMP_DIR/minishell_exit_5.txt" || fail_count=$((fail_count + 1))

    # Test 6: Exporting a non-existent variable (should create and export with empty value)
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_6; echo \"\$VAR_TEST_6\"" "$TEMP_DIR/bash_out_6.txt" "$TEMP_DIR/bash_exit_6.txt"
    run_minishell_test "export VAR_TEST_6; echo \"\$VAR_TEST_6\"" "$TEMP_DIR/minishell_out_6.txt" "$TEMP_DIR/minishell_exit_6.txt"
    compare_results "T$test_count: Export non-existent VAR_TEST_6" "$TEMP_DIR/bash_out_6.txt" "$TEMP_DIR/minishell_out_6.txt" "$TEMP_DIR/bash_exit_6.txt" "$TEMP_DIR/minishell_exit_6.txt" || fail_count=$((fail_count + 1))

    # Test 7: `export -p` (displaying exported variables)
    # This test primarily checks if `export -p` runs without error and produces some output.
    # Direct output comparison is difficult due to environment differences.
    test_count=$((test_count + 1))
    echo -e "\n--- Test Case: T$test_count: export -p ---"
    run_bash_test "export -p" "$TEMP_DIR/bash_out_7.txt" "$TEMP_DIR/bash_exit_7.txt"
    run_minishell_test "export -p" "$TEMP_DIR/minishell_out_7.txt" "$TEMP_DIR/minishell_exit_7.txt"
    local b_exit=$(cat "$TEMP_DIR/bash_exit_7.txt")
    local m_exit=$(cat "$TEMP_DIR/minishell_exit_7.txt")
    if [ "$b_exit" -eq "$m_exit" ] && [ -s "$TEMP_DIR/minishell_out_7.txt" ]; then
        echo "Exit Status: MATCH ($b_exit)"
        echo "Minishell Output: Not empty (as expected)"
        echo "Result: PASS"
    else
        echo "Exit Status: MISMATCH (Bash: $b_exit, Minishell: $m_exit) OR Minishell output is empty."
        echo "--- Bash Output (partial) ---"
        head -n 5 "$TEMP_DIR/bash_out_7.txt" # Show first 5 lines
        echo "--- Minishell Output (partial) ---"
        head -n 5 "$TEMP_DIR/minishell_out_7.txt" # Show first 5 lines
        echo "Result: FAIL"
        fail_count=$((fail_count + 1))
    fi
    echo ""

    # Test 8: `export -n VAR` (unexporting a variable)
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_8=unexported_value; export -n VAR_TEST_8; echo \$VAR_TEST_8" "$TEMP_DIR/bash_out_8.txt" "$TEMP_DIR/bash_exit_8.txt"
    run_minishell_test "export VAR_TEST_8=unexported_value; export -n VAR_TEST_8; echo \$VAR_TEST_8" "$TEMP_DIR/minishell_out_8.txt" "$TEMP_DIR/minishell_exit_8.txt"
    compare_results "T$test_count: export -n VAR_TEST_8" "$TEMP_DIR/bash_out_8.txt" "$TEMP_DIR/minishell_out_8.txt" "$TEMP_DIR/bash_exit_8.txt" "$TEMP_DIR/minishell_exit_8.txt" || fail_count=$((fail_count + 1))

    # Test 9: Exporting a variable with special characters (needs careful quoting)
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_9='!@#\$%^&*()'; echo \"\$VAR_TEST_9\"" "$TEMP_DIR/bash_out_9.txt" "$TEMP_DIR/bash_exit_9.txt"
    run_minishell_test "export VAR_TEST_9='!@#\$%^&*()'; echo \"\$VAR_TEST_9\"" "$TEMP_DIR/minishell_out_9.txt" "$TEMP_DIR/minishell_exit_9.txt"
    compare_results "T$test_count: Export VAR_TEST_9 with special chars" "$TEMP_DIR/bash_out_9.txt" "$TEMP_DIR/minishell_out_9.txt" "$TEMP_DIR/bash_exit_9.txt" "$TEMP_DIR/minishell_exit_9.txt" || fail_count=$((fail_count + 1))

    echo "===================================================================="
    echo "Starting unset tests..."
    echo "===================================================================="

    # Test 10: Simple unset of an existing exported variable
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_10=unset_me; unset VAR_TEST_10; echo \"\$VAR_TEST_10\"" "$TEMP_DIR/bash_out_10.txt" "$TEMP_DIR/bash_exit_10.txt"
    run_minishell_test "export VAR_TEST_10=unset_me; unset VAR_TEST_10; echo \"\$VAR_TEST_10\"" "$TEMP_DIR/minishell_out_10.txt" "$TEMP_DIR/minishell_exit_10.txt"
    compare_results "T$test_count: Simple unset VAR_TEST_10" "$TEMP_DIR/bash_out_10.txt" "$TEMP_DIR/minishell_out_10.txt" "$TEMP_DIR/bash_exit_10.txt" "$TEMP_DIR/minishell_exit_10.txt" || fail_count=$((fail_count + 1))

    # Test 11: Unsetting a non-existent variable (should not error in bash, exit 0)
    test_count=$((test_count + 1))
    run_bash_test "unset VAR_TEST_11_NON_EXISTENT; echo \$?" "$TEMP_DIR/bash_out_11.txt" "$TEMP_DIR/bash_exit_11.txt"
    run_minishell_test "unset VAR_TEST_11_NON_EXISTENT; echo \$?" "$TEMP_DIR/minishell_out_11.txt" "$TEMP_DIR/minishell_exit_11.txt"
    compare_results "T$test_count: Unset non-existent VAR_TEST_11" "$TEMP_DIR/bash_out_11.txt" "$TEMP_DIR/minishell_out_11.txt" "$TEMP_DIR/bash_exit_11.txt" "$TEMP_DIR/minishell_exit_11.txt" || fail_count=$((fail_count + 1))

    # Test 12: Unsetting multiple variables
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_12a=val1; export VAR_TEST_12b=val2; unset VAR_TEST_12a VAR_TEST_12b; echo \"\$VAR_TEST_12a \$VAR_TEST_12b\"" "$TEMP_DIR/bash_out_12.txt" "$TEMP_DIR/bash_exit_12.txt"
    run_minishell_test "export VAR_TEST_12a=val1; export VAR_TEST_12b=val2; unset VAR_TEST_12a VAR_TEST_12b; echo \"\$VAR_TEST_12a \$VAR_TEST_12b\"" "$TEMP_DIR/minishell_out_12.txt" "$TEMP_DIR/minishell_exit_12.txt"
    compare_results "T$test_count: Unset multiple VAR_TEST_12a VAR_TEST_12b" "$TEMP_DIR/bash_out_12.txt" "$TEMP_DIR/minishell_out_12.txt" "$TEMP_DIR/bash_exit_12.txt" "$TEMP_DIR/minishell_exit_12.txt" || fail_count=$((fail_count + 1))

    # Test 13: Unset a non-exported variable (should still unset it)
    test_count=$((test_count + 1))
    run_bash_test "VAR_TEST_13=local_val; unset VAR_TEST_13; echo \"\$VAR_TEST_13\"" "$TEMP_DIR/bash_out_13.txt" "$TEMP_DIR/bash_exit_13.txt"
    run_minishell_test "VAR_TEST_13=local_val; unset VAR_TEST_13; echo \"\$VAR_TEST_13\"" "$TEMP_DIR/minishell_out_13.txt" "$TEMP_DIR/minishell_exit_13.txt"
    compare_results "T$test_count: Unset non-exported VAR_TEST_13" "$TEMP_DIR/bash_out_13.txt" "$TEMP_DIR/minishell_out_13.txt" "$TEMP_DIR/bash_exit_13.txt" "$TEMP_DIR/minishell_exit_13.txt" || fail_count=$((fail_count + 1))

    # Test 14: Unset with arguments that are not valid variable names (bash gives error, exit 1)
    test_count=$((test_count + 1))
    run_bash_test "unset 1VAR_INVALID; echo \$?" "$TEMP_DIR/bash_out_14.txt" "$TEMP_DIR/bash_exit_14.txt"
    run_minishell_test "unset 1VAR_INVALID; echo \$?" "$TEMP_DIR/minishell_out_14.txt" "$TEMP_DIR/minishell_exit_14.txt"
    compare_results "T$test_count: Unset invalid name 1VAR_INVALID" "$TEMP_DIR/bash_out_14.txt" "$TEMP_DIR/minishell_out_14.txt" "$TEMP_DIR/bash_exit_14.txt" "$TEMP_DIR/minishell_exit_14.txt" || fail_count=$((fail_count + 1))

    # Test 15: Unset with no arguments (bash exit 0)
    test_count=$((test_count + 1))
    run_bash_test "unset; echo \$?" "$TEMP_DIR/bash_out_15.txt" "$TEMP_DIR/bash_exit_15.txt"
    run_minishell_test "unset; echo \$?" "$TEMP_DIR/minishell_out_15.txt" "$TEMP_DIR/minishell_exit_15.txt"
    compare_results "T$test_count: Unset with no arguments" "$TEMP_DIR/bash_out_15.txt" "$TEMP_DIR/minishell_out_15.txt" "$TEMP_DIR/bash_exit_15.txt" "$TEMP_DIR/minishell_exit_15.txt" || fail_count=$((fail_count + 1))

    # Test 16: Overwriting an exported variable (should remain exported with new value)
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_16=initial; export VAR_TEST_16=new_value; echo \$VAR_TEST_16" "$TEMP_DIR/bash_out_16.txt" "$TEMP_DIR/bash_exit_16.txt"
    run_minishell_test "export VAR_TEST_16=initial; export VAR_TEST_16=new_value; echo \$VAR_TEST_16" "$TEMP_DIR/minishell_out_16.txt" "$TEMP_DIR/minishell_exit_16.txt"
    compare_results "T$test_count: Overwriting an exported variable" "$TEMP_DIR/bash_out_16.txt" "$TEMP_DIR/minishell_out_16.txt" "$TEMP_DIR/bash_exit_16.txt" "$TEMP_DIR/minishell_exit_16.txt" || fail_count=$((fail_count + 1))

    # Test 17: Exporting a variable with numeric value
    test_count=$((test_count + 1))
    run_bash_test "export VAR_TEST_17=12345; echo \$VAR_TEST_17" "$TEMP_DIR/bash_out_17.txt" "$TEMP_DIR/bash_exit_17.txt"
    run_minishell_test "export VAR_TEST_17=12345; echo \$VAR_TEST_17" "$TEMP_DIR/minishell_out_17.txt" "$TEMP_DIR/minishell_exit_17.txt"
    compare_results "T$test_count: Export numeric VAR_TEST_17" "$TEMP_DIR/bash_out_17.txt" "$TEMP_DIR/minishell_out_17.txt" "$TEMP_DIR/bash_exit_17.txt" "$TEMP_DIR/minishell_exit_17.txt" || fail_count=$((fail_count + 1))

    # Test 18: Unsetting a variable that was never set
    test_count=$((test_count + 1))
    run_bash_test "unset VAR_TEST_18_NEVER_SET; echo \$?" "$TEMP_DIR/bash_out_18.txt" "$TEMP_DIR/bash_exit_18.txt"
    run_minishell_test "unset VAR_TEST_18_NEVER_SET; echo \$?" "$TEMP_DIR/minishell_out_18.txt" "$TEMP_DIR/minishell_exit_18.txt"
    compare_results "T$test_count: Unset never-set VAR_TEST_18" "$TEMP_DIR/bash_out_18.txt" "$TEMP_DIR/minishell_out_18.txt" "$TEMP_DIR/bash_exit_18.txt" "$TEMP_DIR/minishell_exit_18.txt" || fail_count=$((fail_count + 1))

    echo "===================================================================="
    echo "Test Summary:"
    echo "Total tests run: $test_count"
    echo "Tests failed: $fail_count"
    echo "===================================================================="

    if [ "$fail_count" -gt 0 ]; then
        echo "Some tests FAILED. Check the output above and files in '$TEMP_DIR' for details."
        return 1
    else
        echo "All tests PASSED!"
        return 0
    fi
}

# --- Execute tests and cleanup ---
run_tests
test_status=$?

cleanup_test_environment

exit $test_status