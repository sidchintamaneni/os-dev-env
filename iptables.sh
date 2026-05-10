#!/bin/bash

echo "========================================"
echo "nftables Soft Lockup Test"
echo "========================================"
echo "Kernel: $(uname -r)"
echo "iptables: $(iptables --version)"
echo ""

# Enable watchdog
echo 20 > /proc/sys/kernel/watchdog_thresh
echo 1 > /proc/sys/kernel/softlockup_all_cpu_backtrace

# Flush existing rules
nft flush ruleset

# Create test table
nft add table ip test

echo "Creating chains with JUMP rules..."
# Create chains that jump to each other
for i in $(seq 1 1000); do
    nft add chain ip test chain_$i
    
    # Add jump rules (creates validation recursion)
    for j in $(seq 1 20); do
        TARGET=$(((i + j) % 1000 + 1))
        nft add rule ip test chain_$i \
            ip saddr 10.0.$((i % 256)).$j \
            counter \
            jump chain_$TARGET
    done
    
    [ $((i % 100)) -eq 0 ] && echo "  Created $i chains..."
done

TOTAL=$(nft list ruleset | wc -l)
echo ""
echo "Total rules: $TOTAL"

# Save ruleset
echo "Saving ruleset..."
nft list ruleset > /tmp/huge.nft

# This is where the soft lockup happens!
echo ""
echo "========================================"
echo "Triggering validation (watch for hang)..."
echo "========================================"
nft flush ruleset

echo "Restoring... (this should cause soft lockup)"
time nft -f /tmp/huge.nft

echo ""
echo "Checking dmesg for soft lockups..."
dmesg | grep -i "soft lockup" | tail -20

echo ""
echo "Done!"
