CC		:= gcc
CFLAGS	:= -Wall -Wextra -Wpedantic

TARGET	:= main
SRCS	:= $(wildcard *.c)
OBJS	:= $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.0: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
