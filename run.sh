#!/bin/bash
clear
rm bin/main
if [ "$(make)" ]; then
  bin/main
fi
