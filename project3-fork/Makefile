CC = gcc
CFLAGS = -Wall -Wfloat-equal -Wwrite-strings -O2 -pipe
OBJ = max.o main.o
DEPS = max.h
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