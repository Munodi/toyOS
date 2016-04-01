for i in {0..255}
do
	if [ "$i" -eq 8 ] || [
		then
			echo "ISR_ERRCODE $i"
	else
		#then
			echo "ISR_NOERRCODE $i"
	fi
done
