FILENAME1 = test.txt
FILENAME2 = back.txt 
PORT = 4950 
SENDER = localhost 
RECEIVER = localhost
GCC = gcc
WALL = -Wall
LZ = -lz
all : sender.c receiver.c
	$(GCC) sender.c $(WALL) -o sender $(LZ)
	$(GCC) receiver.c $(WALL) -o receiver $(LZ)
	
sender : sender.c 
	$(GCC) sender.c $(WALL) -o sender $(LZ)
	./sender --file $(FILENAME1) --sber 10 --splr 40 --delay 100 $(SENDER) $(PORT)
	
receiver : receiver.c
	> $(FILENAME2)
	$(GCC) receiver.c $(WALL) -o receiver $(LZ)
	./receiver --file $(FILENAME2) $(RECEIVER) $(PORT)

	
clean : 
	rm receiver
	rm sender	
