

.PHONY : all clean

#
# compling 
#
CC         := gcc 
FLAGS      := -Wall -O2 -I.
LIBFLAGS   := -L. 

#
# source
#
SRC := AT.c
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)
PRE := $(SRC:.c=.i)

TARGET := AT

all : $(PRE) $(TARGET)

$(TARGET) : $(OBJ) 
	$(CC) -o $@ $^ $(FLAGS) $(LIBFLAGS) 

%.o : %.c Makefile
	@echo "compling $<..."
	$(CC) -o $@ -c $< $(FLAGS)

%.i : %.c Makefile
	$(CC) -o $@ -E $< $(FLAGS) 

%.d : %.c
	$(CC) -M $< > $@ $(FLAGS) 
	@sed -i "s/^[ \t]*\(.*\)\.o[ \t]*:/\1.o \1.d :/g" $@
#     @sed "s/^[ \t]\{0,\}\(.\{0,\}\)\.o[ \t]\{0,\}:/\1.o \1.d :/g" $@


clean:
	@for File in $(TARGET) $(OBJ) $(DEP) $(PRE);\
            do\
                if [ -e $${File} ] ; then\
                    rm $${File};\
                fi ;\
            done;

-include $(DEP)
