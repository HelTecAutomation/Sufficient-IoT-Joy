.PHONY:clean

OBJS = main.o spi.o rgb_control.o  bh1750.o i2c.o

main: $(OBJS)
%.o : %.c
	gcc -c $^ -o $@

clean:
	rm -rf $(OBJS) main