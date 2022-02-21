CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wshadow -Wstrict-overflow=2 -Wwrite-strings -Wswitch-default -Wswitch-enum -O2 -pipe
OBJ = JacksonMiller.LargestInt.o largestInteger-driver.o
DEPS = listSearcher.h
TARGET = int-search

# Recompile objects when any source or header changes
%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Make the main executable from the 
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: clean

clean:
	rm $(OBJ) $(TARGET)