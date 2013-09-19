#!/bin/bash

make

actual=$(./tokenizer "\n" "hello\nworld")

expect="hello
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 1"
else
  echo "failed 1"
fi

actual=$(./tokenizer "m" "mmmmm")

expect=""

if [ "$actual" == "$expect" ]; then
  echo "passed 2"
else
  echo "failed 2"
fi

actual=$(./tokenizer "e\v\t " "hel\vlo \tworld")

expect="h
l
lo
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 3"
else
  echo "failed 3"
fi

actual=$(./tokenizer "" "You have to print out hex of \a")

expect="You have to print out hex of [0x07]"

if [ "$actual" == "$expect" ]; then
  echo "passed 4"
else
  echo "failed 4"
  echo "actual: 
$actual"
  echo "expect: 
$expect"
fi

actual=$(./tokenizer "\n" "")
expect=""

if [ "$actual" == "$expect" ]; then
  echo "passed 5"
else
  echo "failed 5"
fi

actual=$(./tokenizer "\k" "hello\kworld")

expect="hello
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 6"
else
  echo "failed 6"
fi

actual=$(./tokenizer "k" "hello\kworld")

expect="hello
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 7"
else
  echo "failed 7"
fi

actual=$(./tokenizer "\\\\" "\\\\\\hello\\\\\\world\\\\")

expect="hello
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 8"
else
  echo "failed 8"
fi

actual=$(./tokenizer "\b\le" "ShE sells sEa shells by the sEa shorE\\")

expect="ShE s
s sEa sh
s by th
 sEa shorE"

if [ "$actual" == "$expect" ]; then
  echo "passed 9"
else
  echo "failed 9"
fi

actual=$(./tokenizer "\k\\" "hello\kworld")

expect="hello
world"

if [ "$actual" == "$expect" ]; then
  echo "passed 10"
else
  echo "failed 10"
fi

# actual=$(./tokenizer "" "\\\"")

make clean