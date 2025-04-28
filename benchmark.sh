#!/bin/bash

# Paths
PCAP_DIRECTORY="./tests/cfgs/default/pcap"
OUTPUT_DIRECTORY="./output"

# Number of top functions to extract
K=10

# Create output directory if it doesn't exist
mkdir -p "${OUTPUT_DIRECTORY}"

# Loop over .pcap and .pcapng files
for file_path in "${PCAP_DIRECTORY}"/*.{pcap,pcapng}; do
    # Skip if no matching files
    [ -e "$file_path" ] || continue

    # Get just the base file name (without directory path)
    file_name=$(basename "$file_path")

    # Create a folder for this file inside the output directory
    mkdir -p "${OUTPUT_DIRECTORY}/${file_name}"

    echo "Processing $file_name..."

    # Step 1: Run ndpiReader with perf record
    sudo perf record -o "${OUTPUT_DIRECTORY}/${file_name}/perf.data" ./example/ndpiReader -i "$file_path" >"${OUTPUT_DIRECTORY}/${file_name}/ndpi.pcap.out"

    # Step 2: Run perf report and save top K overhead functions
    sudo perf report --stdio --dsos ndpiReader --input "${OUTPUT_DIRECTORY}/${file_name}/perf.data" |
        awk '/cpu_core\/cycles\/P/ {for (i=0; i<(15); i++) {getline; if(i>=5) print}}' \
            >"${OUTPUT_DIRECTORY}/${file_name}/top${K}_funcs.txt"

    echo "Finished processing $file_name"

done