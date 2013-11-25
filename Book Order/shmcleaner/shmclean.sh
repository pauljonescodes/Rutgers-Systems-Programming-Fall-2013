for i in $(ipcs -m | grep am1471); do
	ipcrm -m $i
done
