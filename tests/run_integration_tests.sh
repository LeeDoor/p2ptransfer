exec=$1/server/p2plisten
echo "running integration tests on $1. executable is $exec"

echo "TEST #1"
echo "Should return immediately when passed port is not a number: "
echo 'notnumber' | $exec 
