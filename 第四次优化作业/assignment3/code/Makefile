ALL: ./bin/wireroute

./bin/wireroute: ./wireroute.cc
	g++ ./wireroute.cc -o ./bin/wireroute -std=c++17 -Wall -O2


clean:
	rm -f ./build/*.o
	rm -f ./bin/wireroute

profile: ./bin/wireroute
	cd ./bin && flamegraph -o graph.svg -- ./wireroute -i 20000 -f ../inputs/testinput/easy_64.txt && firefox ./graph.svg

run: ./bin/wireroute
	cd ./bin && ./wireroute -i 20000 -f ../inputs/testinput/easy_64.txt
