.PHONY:clean

OBJS = main.o spi.o rgb_control.o 

main: $(OBJS)
%.o : %.c
	gcc -c $^ -o $@

clean:
	rm -rf $(OBJS) main