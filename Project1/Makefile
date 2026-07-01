all: schedule two

schedule: schedule.c
	gcc -Wall -Werror -g -std=c99 -o $@ $?

two: two.c
	gcc -Wall -Werror -g -std=c99 -o $@ $?

clean:
	-rm -rf schedule two
