#!/bin/bash

# Check if an argument was provided
if [ -z "$1" ]; then
    echo "Usage: $0 <filename.h>"
    exit 1
fi

# Get the filename and convert it to uppercase for the guard
# Example: uart.h -> UART_H
FILENAME=$1
GUARD_NAME="${FILENAME//./_}"
GUARD_NAME="${GUARD_NAME^^}"

# Create the file and write the template
cat <<EOF > "$FILENAME"
#ifndef ${GUARD_NAME}
#define ${GUARD_NAME}

#ifdef __cplusplus
extern "C" {
#endif

/* Your code here */

#ifdef __cplusplus
}
#endif

#endif /* ${GUARD_NAME} */
EOF

echo "Created header file: $FILENAME"
