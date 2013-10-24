#!/bin/bash
clear
if [ "$(make)" ]; then
  bin/main $1
fi
