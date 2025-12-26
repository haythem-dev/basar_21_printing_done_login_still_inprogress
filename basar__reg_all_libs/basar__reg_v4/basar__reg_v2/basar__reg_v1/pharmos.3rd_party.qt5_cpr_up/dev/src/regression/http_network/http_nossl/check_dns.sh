
#!/bin/bash

echo "=== DNS and Network Connectivity Check ==="
echo "Date: $(date)"
echo "=========================================="

# Check if we can resolve hostnames
echo "Testing DNS resolution:"
echo "----------------------"

hosts=(httpbin.org jsonplaceholder.typicode.com httpbingo.org)
total_hosts=${#hosts[@]}
current=0

for host in "${hosts[@]}"; do
    current=$((current + 1))
    
    # Progress bar
    progress=$((current * 100 / total_hosts))
    bar_length=20
    filled_length=$((progress * bar_length / 100))
    
    printf "\rProgress: ["
    for ((i=0; i<filled_length; i++)); do printf "█"; done
    for ((i=filled_length; i<bar_length; i++)); do printf "░"; done
    printf "] %d%% (%d/%d)" $progress $current $total_hosts
    
    echo ""
    echo -n "Resolving $host: "
    if nslookup $host >/dev/null 2>&1; then
        ip=$(nslookup $host 2>/dev/null | grep "Address:" | tail -1 | awk '{print $2}')
        echo "✓ $ip"
    else
        echo "✗ Failed"
    fi
done

echo ""

echo ""
echo "Testing HTTP connectivity:"
echo "-------------------------"

for url in "http://httpbin.org/get" "http://jsonplaceholder.typicode.com/posts/1" "http://httpbingo.org/get"; do
    echo -n "Testing $url: "
    if curl -s --connect-timeout 5 --max-time 10 "$url" >/dev/null 2>&1; then
        echo "✓ Reachable"
    else
        echo "✗ Unreachable"
    fi
done

echo ""
echo "DNS Configuration:"
echo "-----------------"
if [ -f /etc/resolv.conf ]; then
    echo "Current DNS servers:"
    grep nameserver /etc/resolv.conf
else
    echo "No /etc/resolv.conf found"
fi

echo ""
echo "Network interface status:"
echo "------------------------"
netstat -rn | head -5

echo "=========================================="
