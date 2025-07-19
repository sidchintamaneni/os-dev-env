#!/bin/bash
# With the assistance of AI

ITERATIONS=1000
COMMAND="hackbench -s 2048 -l 1000 -g 30 -f 25 -P"

echo "Running hackbench benchmark $ITERATIONS times..."
echo "Command: $COMMAND"

# Create array to store times
declare -a times

# Progress counter
count=0
start_time=$(date +%s)

for ((i=1; i<=$ITERATIONS; i++)); do
  # Run hackbench and capture its output
  output=$($COMMAND 2>&1)
  
  # Extract the time value (assuming it outputs "Time: X.XXX")
  time_value=$(echo "$output" | grep -oP 'Time:\s+\K[0-9.]+')
  
  # Store the time value
  times+=($time_value)
  
  # Update progress every 10 iterations
  count=$((count+1))
  if (( count % 10 == 0 )) || (( count == ITERATIONS )); then
    elapsed=$(($(date +%s) - start_time))
    echo -ne "Progress: $count/$ITERATIONS ($(($count*100/ITERATIONS))%) - Elapsed: ${elapsed}s\r"
  fi
done

echo -e "\nCalculating statistics..."

# Calculate sum
sum=0
for t in "${times[@]}"; do
  sum=$(echo "$sum + $t" | bc -l)
done

# Calculate average
avg=$(echo "scale=6; $sum / $ITERATIONS" | bc -l)

# Calculate min and max
IFS=$'\n'
min=$(echo "${times[*]}" | sort -n | head -n1)
max=$(echo "${times[*]}" | sort -n | tail -n1)

# Calculate standard deviation
sum_sq_diff=0
for t in "${times[@]}"; do
  diff=$(echo "$t - $avg" | bc -l)
  sq_diff=$(echo "$diff * $diff" | bc -l)
  sum_sq_diff=$(echo "$sum_sq_diff + $sq_diff" | bc -l)
done
std_dev=$(echo "scale=6; sqrt($sum_sq_diff / $ITERATIONS)" | bc -l)

# Print results
echo "=== Results ==="
echo "Iterations: $ITERATIONS"
echo "Average time: $avg seconds"
echo "Minimum time: $min seconds"
echo "Maximum time: $max seconds"
echo "Standard deviation: $std_dev seconds"

# Save results to file
results_file="hackbench_results_$(date +%Y%m%d_%H%M%S).txt"
{
  echo "Command: $COMMAND"
  echo "Iterations: $ITERATIONS"
  echo "Average time: $avg seconds"
  echo "Minimum time: $min seconds"
  echo "Maximum time: $max seconds" 
  echo "Standard deviation: $std_dev seconds"
  echo -e "\nRaw data (seconds):"
  printf "%s\n" "${times[@]}"
} > "$results_file"

echo "Results saved to $results_file"
