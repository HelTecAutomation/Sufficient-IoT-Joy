.PHONY:clean

OBJS = main.o spi.o

main: $(OBJS)
%.o : %.c
	gcc -c $^ -o $@

clean:
	rm -rf $(OBJS) main