CC = gcc

.PHONY : all clean run swap end

CFLAGS = -pedantic -Wall -Wextra -g

all : main

run : main
	./main < data.in

main :

swap :
	cp ./main.c ./main1.c
	git add ./main1.c

end :
	cp ./main.c ./main2.c
	git add ./main2.c

clean :
	rm main

