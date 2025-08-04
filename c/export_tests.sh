#!/bin/bash
# filepath: /mnt/c/Users/user/Desktop/Minishell/export_tests.sh

MINISHELL=./minishell
TMP1=$(mktemp)
TMP2=$(mktemp)
BASH_ENV=$(mktemp)
MINI_ENV=$(mktemp)

# Function to test a command
test_command() {
    local test_name="$1"
    local command="$2"
    local expected_status="$3"
    
    echo "Testing: $test_name"
    echo "Command: $command"
    
    # Bash output
    bash -c "$command" > "$TMP1" 2>&1
    bash_status=$?
    
    # Minishell output
    echo "$command" | timeout 5s $MINISHELL > "$TMP2" 2>&1
    mini_status=$?
    
    # Compare output and status
    if ! diff -q "$TMP1" "$TMP2" >/dev/null || [ "$bash_status" -ne "$mini_status" ]; then
        echo "❌ FAILED"
        echo "Expected status: $expected_status, Bash: $bash_status, Minishell: $mini_status"
        echo "--- Bash Output ---"
        cat "$TMP1"
        echo "--- Minishell Output ---"
        cat "$TMP2"
        echo "================================================"
    else
        echo "✅ PASSED"
        echo ""
    fi
}

# Function to test multi-line commands
test_multiline_command() {
    local test_name="$1"
    local commands="$2"
    local expected_status="$3"
    
    echo "Testing: $test_name"
    echo "Commands: $commands"
    
    # Create temp files for bash and minishell scripts
    echo "$commands" > bash_script.tmp
    echo -e "$commands\nexit" > mini_script.tmp
    
    # Bash output
    bash < bash_script.tmp > "$TMP1" 2>&1
    bash_status=$?
    
    # Minishell output
    timeout 10s $MINISHELL < mini_script.tmp > "$TMP2" 2>&1
    mini_status=$?
    
    # Clean up temp files
    rm -f bash_script.tmp mini_script.tmp
    
    # Compare output and status
    if ! diff -q "$TMP1" "$TMP2" >/dev/null || [ "$bash_status" -ne "$mini_status" ]; then
        echo "❌ FAILED"
        echo "Expected status: $expected_status, Bash: $bash_status, Minishell: $mini_status"
        echo "--- Bash Output ---"
        cat "$TMP1"
        echo "--- Minishell Output ---"
        cat "$TMP2"
        echo "================================================"
    else
        echo "✅ PASSED"
        echo ""
    fi
}

echo "=== EXPORT TESTS ==="

# Export Test 1: Multiple exports with different formats
test_command "Export multiple variables" 'export x1="abcd" x2="123" x3="shoaib"' 0

# Export Test 2: Invalid identifier
test_command "Export invalid identifier" 'export 1x' 1

# Export Test 3: Mixed valid/invalid identifiers
test_command "Export mixed identifiers" 'export test="123" 1test="test" test3="abcd"' 1

# Export Test 4: Empty export
test_command "Export without assignment" 'export test1' 0

# Export Test 5: Empty value assignment
test_command "Export empty value" 'export test2=' 0

# Export Test 6: Variable expansion in export (multiline)
test_multiline_command "Export with variable expansion" "export var=a
export \$var=test
echo \$var \$a" 0

# Export Test 7: Export with HOME expansion (multiline)
test_multiline_command "Export with HOME" "export test=\$HOME
echo \$test" 0

# Export Test 8: Export with spaces (multiline)
test_multiline_command "Export with leading space" "export x=\" s -l -a \"
ls\$x" 0

# Export Test 9: Export with trailing space (multiline)
test_multiline_command "Export with trailing space" "export x=\"s -l -a\"
l\$x" 127

# Export Test 10: Export non-existent variable
test_command "Export non-existent variable" 'export $non_exist' 0

# Export Test 11: Export empty string identifier
test_command "Export empty identifier" 'export ""' 1

# Export Test 12: Export with quotes (multiline)
test_multiline_command "Export with quotes" "export var=\" shoaib\"
echo \$var | cat -e" 0

# Export Test 13: Export complex quoting (multiline)
test_multiline_command "Export complex quotes" "export test=\" shoaib ft \"
echo \"\"\$test\"\"" 0

# Export Test 14: Export with equal in value (multiline)
test_multiline_command "Export with equal in value" "export \"test=shoaib\"=ft
echo \$test" 0

echo ""
echo "=== UNSET TESTS ==="

# Unset Test 1: Unset single variable (multiline)
test_multiline_command "Unset single variable" "export USER=testuser
unset USER
echo \$USER" 0

# Unset Test 2: Unset multiple variables (multiline)
test_multiline_command "Unset multiple variables" "export USER=test
export PATH=/test
unset USER PATH
echo \$USER\$PATH" 0

# Unset Test 3: Unset with empty string
test_command "Unset empty string" 'unset "" test' 0

# Unset Test 4: Unset with single quotes
test_command "Unset single quotes" "unset '' test" 0

# Unset Test 5: Unset non-existent variable
test_command "Unset non-existent" 'unset non_existent_var' 0

echo ""
echo "=== EXPORT WITH EXPANDER TESTS ==="

# Export Expander Test 1: Variable with spaces (multiline)
test_multiline_command "Export variable with spaces" "export var=\" shoaib\"
echo \$var | cat -e" 0

# Export Expander Test 2: Variable concatenation (multiline)
test_multiline_command "Export variable concatenation" "export test=\"Makefile\"
echo \"\$test\"\"Makefile\"" 0

# Export Expander Test 3: Complex variable expansion (multiline)
test_multiline_command "Complex variable expansion" "export test=\" shoaib ft \"
echo ab\$test" 0

# Export Expander Test 4: Multiple expansions (multiline)
test_multiline_command "Multiple expansions" "export test=\" abcd 123 \"
echo \$test \"\$test\"" 0

# Export Expander Test 5: Variable with trailing space (multiline)
test_multiline_command "Variable with trailing space" "export x=\" ismail\"
echo \$x" 0

# Export Expander Test 6: Variable in quotes (multiline)
test_multiline_command "Variable in quotes" "export x=\" ismail\"
echo \"\$x\"" 0

# Export Expander Test 7: Empty quotes with variable (multiline)
test_multiline_command "Empty quotes with variable" "export x=\" ismail\"
echo \"\"\$x\"\"" 0

# Export Expander Test 8: Variable concatenation without quotes (multiline)
test_multiline_command "Variable concatenation no quotes" "export tests=\"\"
echo \"\$tests\"Makefile" 0

# Export Expander Test 9: Variable with space separation (multiline)
test_multiline_command "Variable with space separation" "export tests=\"\"
echo \"\$tests\" \"Makefile\"" 0

# Export Expander Test 10: Export with spaces around text (multiline)
test_multiline_command "Export with spaces around text" "export test=\" shoaib ft \"
echo \$test" 0

# Export Expander Test 11: Export with prefix (multiline)
test_multiline_command "Export with prefix" "export test=\" shoaib ft \"
echo ab\$test" 0

# Export Expander Test 12: Export with quoted prefix (multiline)
test_multiline_command "Export with quoted prefix" "export test=\" shoaib ft \"
echo \"ab\"\$test" 0

# Export Expander Test 13: Export with triple quotes (multiline)
test_multiline_command "Export with triple quotes" "export test=\" shoaib ft \"
echo \"\"\"\$test\"\"\"" 0

# Export Expander Test 14: Export space variable multiple usage (multiline)
test_multiline_command "Export space variable multiple usage" "export test=\" \"
echo \$test \$test \$test abcd \$non_exist \$test 123 \$test" 0

# Export Expander Test 15: Complex expansion tests (multiline)
test_multiline_command "Complex expansion test 1" "export test=\" abcd 123 \"
echo \$test \"\$test\"" 0

test_multiline_command "Complex expansion test 2" "export test=\" abcd 123 \"
echo \$test\"\$test\"" 0

test_multiline_command "Complex expansion test 3" "export test=\" abcd 123 \"
echo \"\$test\" \$test" 0

test_multiline_command "Complex expansion test 4" "export test=\" abcd 123 \"
echo \"\$test\"\$test" 0

test_multiline_command "Complex expansion test 5" "export test=\" abcd 123 \"
echo \"\$test\"v\$test" 0

test_multiline_command "Complex expansion test 6" "export test=\" abcd 123 \"
echo \$testv\"\$test\"" 0

rm "$TMP1" "$TMP2" "$BASH_ENV" "$MINI_ENV"

echo ""
echo "=== TEST SUMMARY ==="
echo "All export and unset tests completed!"
echo "Check the results above for any failures."