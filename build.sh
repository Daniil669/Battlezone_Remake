#!/bin/bash

set -euo pipefail # exit on fail

gcc -Wall -Wextra -g -std=c11 ./src/main.c ./src/db/scores.c -o battlezone $(pkg-config --cflags --libs sqlite3) # -g remove for prod

# make dev and prod version of this build script (configs settings hardcode)
# devide command into variables
# include packages later $(pkg-config --cflag --libs sdl2 sqlite3)
# include fallback for packages
# possible include -MMD dependecy files
# make game build and install or uninstall so it can be ran from terminal

if [ $? -eq 0 ]; then
    echo "Build OK -> ./battlezone"
    ./battlezone
else
    echo "Build failed"
fi