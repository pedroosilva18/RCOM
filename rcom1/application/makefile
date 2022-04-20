CC = gcc
C_FLAGS = -g -Wall

gcc:
	$(CC) $(C_FLAGS) -c linklayer.c aux.c data.c
	$(CC) main.c linklayer.o aux.o data.o -o app

tx:
	./app /dev/ttyS10 tx penguin.gif

rx:
	./app /dev/ttyS11 rx penguin-recieved.gif

txlab:
	./app /dev/ttyS0 tx penguin.gif

rxlab:
	./app /dev/ttyS1 rx penguin-recieved.gif

tx_fine:
	./app /dev/ttyS10 tx this_is_fine.gif

tx_png:
	./app /dev/ttyS10 tx rubiks_cube.png

tx_jpg:
	./app /dev/ttyS10 tx Foto_CV.jpg

tx_txt:
	./app /dev/ttyS10 tx test.txt

rx_fine:
	./app /dev/ttyS11 rx super_fine.gif

rx_png:
	./app /dev/ttyS11 rx rubiks_cube_r.png

rx_jpg:
	./app /dev/ttyS11 rx Foto_CV_r.jpg

rx_txt:
	./app /dev/ttyS11 rx test-r.txt

clear:
	rm -f app a.out
	rm -f *.o
	rm -f penguin-recieved.gif
	rm -f Foto_CV_r.gif
	rm -f rubiks_cube_r.png
	rm -f super_fine.gif