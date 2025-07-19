#!/bin/bash
# With the assistance of AI

# Configuration - modify these as needed
ITERATIONS=1  # Set to 1 initially for testing
COMMAND="hackbench -s 2048 -l 1000 -g 30 -f 25 -P"
SCHEDULERS_PATH=""  # Will be provided as an argument

# Check for path argument
if [ -z "$1" ]; then
  echo "Usage: $0 <path-to-schedulers>"
  exit 1
else
  SCHEDULERS_PATH="$1"
  if [ ! -d "$SCHEDULERS_PATH" ]; then
    echo "Error: Directory $SCHEDULERS_PATH does not exist"
    exit 1
  fi
fi

echo "Starting sched_ext scheduler benchmark..."

# Get list of schedulers (excluding scx_simple, scxctl and scxtop)
scheduler_files=$(find "$SCHEDULERS_PATH" -type f -executable | grep -v "scx_simple" | grep -v "scxctl" | grep -v "scxtop" | grep -v "vmlinux_docify" | grep -v  "scx_chaos")

if [ -z "$scheduler_files" ]; then
  echo "No executable scheduler files found in $SCHEDULERS_PATH"
  exit 1
fi

# Get scheduler names
all_schedulers=$(basename -a $scheduler_files)

# Results directory
results_dir="sched_ext_benchmark_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$results_dir"

# Function to test a single scheduler
test_scheduler() {
  scheduler=$1
  echo -e "\n\n============================================="
  echo "Testing scheduler: $scheduler"
  echo "============================================="
  
  # Load scheduler
  echo "Running scheduler: $scheduler"
  # Execute the binary directly and store the PID, redirect all output to /dev/null
  "$SCHEDULERS_PATH/$scheduler" > /dev/null 2>&1 &
  scheduler_pid=$!
  
  # Wait for scheduler to initialize
  sleep 2
  
  # Verify scheduler is running
  if ! ps -p $scheduler_pid > /dev/null; then
    echo "Scheduler $scheduler is not running. Skipping."
    return 1
  fi
  
  # Create array to store times
  declare -a times
  
  # Progress counter
  count=0
  start_time=$(date +%s)
  
  echo "Running hackbench benchmark $ITERATIONS times..."
  echo "Command: $COMMAND"
  
  for ((i=1; i<=$ITERATIONS; i++)); do
    # Run hackbench and capture its complete output
    output=$($COMMAND)
    
    # Print the complete hackbench output to terminal
    echo -e "\n--- Hackbench Output ---"
    echo "$output"
    echo "------------------------"
    
    # Extract the time value from hackbench output
    time_value=$(echo "$output" | grep -oP 'Time:\s+\K[0-9.]+')
    
    if [ -n "$time_value" ]; then
      echo "Extracted time: $time_value seconds"
      times+=($time_value)
    else
      echo "WARNING: Could not extract time value from hackbench output."
      echo "Will use 0 as placeholder."
      times+=(0)
    fi
    
    # Update progress every iteration
    count=$((count+1))
    elapsed=$(($(date +%s) - start_time))
    echo "Progress: $count/$ITERATIONS ($(($count*100/ITERATIONS))%) - Elapsed: ${elapsed}s"
  done
  
  echo -e "\nCalculating statistics..."
  
  # Check if we have any valid time measurements
  if [ ${#times[@]} -eq 0 ]; then
    echo "ERROR: No time measurements were collected."
    return 1
  fi
  
  # Calculate sum
  sum=0
  for t in "${times[@]}"; do
    if [ "$t" != "0" ]; then
      sum=$(echo "$sum + $t" | bc -l)
    fi
  done
  
  # Calculate average
  avg=$(echo "scale=6; $sum / ${#times[@]}" | bc -l)
  
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
  std_dev=$(echo "scale=6; sqrt($sum_sq_diff / ${#times[@]})" | bc -l)
  
  # Print results
  echo "=== Results for $scheduler ==="
  echo "Iterations: ${#times[@]}"
  echo "Average time: $avg seconds"
  echo "Minimum time: $min seconds"
  echo "Maximum time: $max seconds"
  echo "Standard deviation: $std_dev seconds"
  
  # Save results to file
  results_file="$results_dir/${scheduler}_results.txt"
  {
    echo "Scheduler: $scheduler"
    echo "Command: $COMMAND"
    echo "Iterations: ${#times[@]}"
    echo "Average time: $avg seconds"
    echo "Minimum time: $min seconds"
    echo "Maximum time: $max seconds" 
    echo "Standard deviation: $std_dev seconds"
    echo -e "\nRaw data (seconds):"
    printf "%s\n" "${times[@]}"
    
    echo -e "\nComplete hackbench output:"
    echo "$output"
  } > "$results_file"
  
  echo "Results saved to $results_file"
  
  # Kill the scheduler process
  echo "Terminating scheduler process (PID: $scheduler_pid)"
  kill $scheduler_pid
  sleep 2
  
  # Make sure it's really dead
  if ps -p $scheduler_pid > /dev/null; then
    echo "Scheduler process didn't terminate normally. Sending SIGKILL."
    kill -9 $scheduler_pid
    sleep 1
  fi
}

# Run tests for each scheduler
for scheduler in $all_schedulers; do
  test_scheduler "$scheduler"
done

# Generate comparative summary
echo -e "\n\nGenerating summary report..."
{
  echo "# sched_ext Scheduler Benchmark Summary"
  echo "Date: $(date)"
  echo "Command: $COMMAND"
  echo "Iterations per scheduler: $ITERATIONS"
  echo -e "\n## Average Execution Time (seconds)"
  echo "| Scheduler | Average | Min | Max | Std Dev |"
  echo "|-----------|---------|-----|-----|---------|"
  
  for scheduler in $all_schedulers; do
    results_file="$results_dir/${scheduler}_results.txt"
    if [ -f "$results_file" ]; then
      avg=$(grep "Average time:" "$results_file" | awk '{print $3}')
      min=$(grep "Minimum time:" "$results_file" | awk '{print $3}')
      max=$(grep "Maximum time:" "$results_file" | awk '{print $3}')
      std=$(grep "Standard deviation:" "$results_file" | awk '{print $3}')
      echo "| $scheduler | $avg | $min | $max | $std |"
    fi
  done
} > "$results_dir/summary.md"

echo -e "\nAll benchmarks completed!"
echo "Summary report saved to: $results_dir/summary.md"
echo "Individual results saved in: $results_dir/"
