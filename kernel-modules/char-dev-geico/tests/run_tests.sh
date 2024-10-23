#!/bin/bash

FOLDER="./" 

for file in "$FOLDER"/*; do
    if [ -x "$file" ] && [ ! -d "$file" ]; then
        ./"$file" 
    fi
done

