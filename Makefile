
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCS = main.c log_sys.c

app:$(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

.PHONY:clean
clean:
	rm -f app
