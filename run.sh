#!/bin/bash
clear
if [ "$(make)" ]; then
  bin/main
fi
