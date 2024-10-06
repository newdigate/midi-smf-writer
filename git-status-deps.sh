#!/bin/bash
find ./cmake-build-debug/_deps -name "*-src" -maxdepth 1 -mindepth 1 -type d -exec sh -c '(echo {} && cd {} && git status -s && echo)' \;
