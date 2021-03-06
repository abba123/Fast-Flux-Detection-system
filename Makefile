CC = g++

File = ASNUtil.cpp ASNUtil.h QueryASN.cpp QueryASN.h RTTQuery.cpp RTTQuery.h RTTUtil.cpp RTTUtil.h Traceroute.cpp Traceroute.h
test_file = test.cpp
Main = F2DS.cpp
URL = metric.nationalgeographic.com

all: F2DS

F2DS: $(Main) $(File) 
		$(CC) $(Main) ASNUtil.cpp QueryASN.cpp RTTQuery.cpp RTTUtil.cpp Traceroute.cpp -lcurl -o F2DS 

test: $(test_file) $(File) 
		$(CC) $(test_file) ASNUtil.cpp QueryASN.cpp RTTQuery.cpp RTTUtil.cpp Traceroute.cpp -lcurl -o test
		sudo ./test

run: F2DS 
		sudo ./F2DS $(URL)

clean: 
		rm F2DS test
