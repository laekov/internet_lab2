## Start two dockers
```
docker run --rm -it --network scope1 -v (pwd):/srv nenv
docker run --privileged --rm -it --network scope0 --ip 192.168.1.10 -v (pwd):/srv nenv
```

## Config route table on tester container
```
route add -net 192.168.0.0/16 gw 192.168.1.2 dev eth0
route del -net 192.168.1.0/24 dev eth0
route del -net 192.168.6.0/24 dev eth1
```
