NAME = hinfosvc
LOGIN = xharma05

all:
	gcc -std=gnu99 $(NAME).c -o hinfosvc

targz:
	tar -czvf $(LOGIN).tar.gz *.c Makefile README.md
