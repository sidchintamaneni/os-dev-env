#!/bin/bash

FOLDER="./"
SCRIPT_NAME=$(basename "$0")

for file in "$FOLDER"/*; do
    if [ -x "$file" ] && [ ! -d "$file" ]; then
        CURRENT_FILE=$(basename "$file")
        if [ "$CURRENT_FILE" != "$SCRIPT_NAME" ]; then
            echo "$CURRENT_FILE"
            "./$CURRENT_FILE"
        fi
    fi
done

