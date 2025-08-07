# #!/bin/bash


export VAR_TEST_1=hello
echo $VAR_TEST_1

VAR_TEST_2=world
export VAR_TEST_2
echo $VAR_TEST_2

export VAR_TEST_3=""
echo "$VAR_TEST_3"

export VAR_TEST_4="value with spaces"
echo "$VAR_TEST_4"

export VAR_TEST_5_ORIGINAL=original_value
export VAR_TEST_5=$VAR_TEST_5_ORIGINAL
echo "$VAR_TEST_5"

export VAR_TEST_6
echo "$VAR_TEST_6"

export -p

export VAR_TEST_9='!@#$%^&*()'
echo "$VAR_TEST_9"

export VAR_TEST_10=unset_me
unset VAR_TEST_10
echo "$VAR_TEST_10"

unset VAR_TEST_11_NON_EXISTENT
echo $?

export VAR_TEST_12a=val1
export VAR_TEST_12b=val2
unset VAR_TEST_12a
unset VAR_TEST_12b
echo "$VAR_TEST_12a $VAR_TEST_12b"

unset
echo $?

export VAR_TEST_16=initial
export VAR_TEST_16=new_value
echo "$VAR_TEST_16"

export VAR_TEST_17=12345
echo $VAR_TEST_17

unset VAR_TEST_18_NEVER_SET
echo $?
