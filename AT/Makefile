

.PHONY : all clean

PATH:=/opt/GCC/4.6.2/linux/bin:$(PATH)
export PATH

ifeq ("$(origin V)", "command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif
ifeq ($(BUILD_VERBOSE),1)
  HIDE=
else
  HIDE= @
endif


#CROSS_COMPILE ?= arm-none-eabi-
CROSS_COMPILE ?=

#
# compling 
#
CC         := $(CROSS_COMPILE)gcc 
CFLAGS      := -Wall -O2
LDFLAGS   := -L.

ifneq ($(CROSS_COMPILE),)
CFLAGS += -I/opt/bionic/libc/include
endif

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
	$(HIDE) $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

%.o : %.c Makefile
	@echo "compling $<..."
	$(HIDE) $(CC) -o $@ $(CFLAGS) -c $<

%.i : %.c Makefile
	$(HIDE) $(CC) -o $@ $(CFLAGS) -E $<

%.d : %.c
	$(HIDE) $(CC) -M $< > $@ $(CFLAGS) 
	$(HIDE) sed -i "s/^[ \t]*\(.*\)\.o[ \t]*:/\1.o \1.d :/g" $@
#     @sed "s/^[ \t]\{0,\}\(.\{0,\}\)\.o[ \t]\{0,\}:/\1.o \1.d :/g" $@


clean:
	$(HIDE) for File in $(TARGET) $(OBJ) $(DEP) $(PRE);\
            do\
                if [ -e $${File} ] ; then\
                    rm $${File};\
                fi ;\
            done;

-include $(DEP)
