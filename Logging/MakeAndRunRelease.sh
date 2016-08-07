#!/usr/bin/env bash
cmake . -DLOGGING_ENABLED=OFF
make
./LoggingExample