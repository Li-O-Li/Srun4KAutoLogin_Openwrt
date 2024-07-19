# Variables

CC = mipsel-openwrt-linux-gcc
# CC = gcc
CFLAGS = -I./includes -Wall -O2
SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
TESTDIR = ./tests
BINADDIX = ""

# main program
TGT = $(BINDIR)/srunRequestBodyGeneration$(BINADDIX)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# dependencies for test builds
DEPOBJS = $(filter-out $(OBJDIR)/main.o, $(OBJS))

# tests
TESTSOURCES = $(wildcard $(TESTDIR)/src/*.c)
TESTOBJS = $(patsubst $(TESTDIR)/src/%.c,$(TESTDIR)/obj/%.o,$(TESTSOURCES))
TESTBINS = $(patsubst $(TESTDIR)/src/%.c,$(TESTDIR)/bin/%$(BINADDIX),$(TESTSOURCES))

# default rule
.PHONY: all clean
all: $(TGT) $(TESTBINS)

# main build
$(TGT): $(OBJS)
	@echo "Linking $(TGT) from $(OBJS)"
	$(CC) $(CFLAGS) $(OBJS) -o $(TGT)


# test build
$(TESTDIR)/bin/%$(BINADDIX): $(TESTDIR)/obj/%.o $(DEPOBJS)
	@echo "Linking $(TESTBINS) from $< and $(DEPOBJS)"
	$(CC) $(CFLAGS) $< $(DEPOBJS) -o $@

$(TESTDIR)/obj/%.o: $(TESTDIR)/src/%.c $(DEPOBJS)
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) -c $< -o $@

# dependencies build
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $^ to $@"
	$(CC) $(CFLAGS) -c $^ -o $@

# clean
clean:
	@echo "Cleaning up..."
	rm -f $(OBJDIR)/*.o $(BINDIR)/*$(BINADDIX)
	rm -f $(TESTDIR)/obj/*.o $(TESTDIR)/bin/*$(BINADDIX)