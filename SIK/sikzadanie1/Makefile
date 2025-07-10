CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=gnu17 -I.
LDFLAGS = -Wall

TARGETS = peer-time-sync test
OBJDIR  = build

# Source files and corresponding object files
SRC     = peer-time-sync.c err.c utils.c message_handler.c
OBJS    = $(SRC:%.c=$(OBJDIR)/%.o)

TEST_SRC  = test.c err.c utils.c message_handler.c
TEST_OBJS = $(TEST_SRC:%.c=$(OBJDIR)/%.o)

all: $(OBJDIR) $(TARGETS)

# Link the final binary
peer-time-sync: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

test: $(TEST_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(TARGETS) *~