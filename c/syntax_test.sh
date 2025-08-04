#!/bin/bash

# Path to your minishell executable
MINISHELL=./minishell

# Temporary files
OUT_FILE=minishell_output.txt
ERR_FILE=minishell_error.txt

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

function run_test() {
    CMD="$1"
    EXPECTED_EXIT="$2"
    DESCRIPTION="$3"

    echo -e "${RED}Test: $DESCRIPTION${NC}"
    echo "$CMD" | $MINISHELL > "$OUT_FILE" 2> "$ERR_FILE"
    ACTUAL_EXIT=$?

    echo -e "Command: \"$CMD\""
    echo "Expected exit code: $EXPECTED_EXIT, Got: $ACTUAL_EXIT"

    if [ "$ACTUAL_EXIT" -eq "$EXPECTED_EXIT" ]; then
        echo -e "${GREEN}✓ Success${NC}"
    else
        echo -e "${RED}✗ Failed${NC}"
    fi
    echo "-----------------------------"
}

# 🧪 Syntax Error Tests
run_test ">" 2 "Single > token (syntax error)"
run_test "<<" 2 "Single << token (syntax error)"
run_test "<<<" 2 "Triple < (syntax error)"
run_test "< >" 2 "Invalid redirection"
run_test "|" 2 "Single | at start"
run_test "|||" 2 "Triple | (syntax error)"
run_test "." 2 "Dot command with no argument"
run_test ".." 127 "Double dots (invalid command)"
run_test "Test/" 127 "Nonexistent folder"
run_test "Makefile/" 126 "Not a directory"
run_test "/" 126 "Directory is not executable"
