all: generator vanish_dir preprocessor generator_master

CC = gcc
CFLAGS = -g -O3 -Wall

generator: generator.c
	gcc -o ./../../generator generator.c
vanish_dir: vanish_dir.c
	gcc -o ./../../vanish_dir vanish_dir.c
preprocessor: preprocessor.c
	gcc -o ./../preprocessor preprocessor.c
generator_master: generator_master.c
	gcc -o ./../generator_master generator_master.c

clear:
	rm -f *.c
