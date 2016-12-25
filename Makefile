CC = g++

File = ASNUtil.cpp ASNUtil.h QueryASN.cpp QueryASN.h
test_file = test.cpp
Main = F2DS.cpp

all: F2DS

F2DS: $(Main) $(File) 
		$(CC) $(Main) ASNUtil.cpp QueryASN.cpp -lcurl -o F2DS

test: $(test_file) $(File) 
		$(CC) $(test_file) ASNUtil.cpp QueryASN.cpp -lcurl -o test
		./test

run: F2DS 
		./F2DS

clean: 
		rm F2DS test
