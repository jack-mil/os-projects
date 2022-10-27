CC = gcc
CFLAGS = -Wall -Wfloat-equal -Wwrite-strings -O2 -pipe
LDLIBS = -lpthread
TARGET = traffic

# Make the main executable from the objects
default : $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS = JacksonMiller5.o
HEADERS = $(wildcard *.h)

# Recompile objects when any source or header changes
%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile executable from objects and linked librarys
$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LDLIBS) -o $@



.PHONY: clean
clean:
	rm $(OBJECTS) $(TARGET)