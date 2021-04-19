myIP=`curl ifconfig.me`

updateStatus=`curl 'https://www.duckdns.org/update?domains=sanhoo&token=<tokenHere>&ip='$myIP`
echo "my IP is " $myIP "update staus" $updateStatus