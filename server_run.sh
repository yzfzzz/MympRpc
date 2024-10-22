#!/bin/bash

if [ -d "logs" ]; then
    echo "logs directory already exists."
    rm -r logs
else
    echo "Created logs directory."
fi

mkdir logs

cd bin
./provider -i test.conf