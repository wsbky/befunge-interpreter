#!/bin/bash

CURRENT=$(
  cd $(dirname $0)
  pwd
)

echo -n "Hello, World\!" >$CURRENT/01_o.txt
echo -n "Thanks\!" >$CURRENT/02_o.txt
