CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=gnu17 -I.
LDFLAGS =

TARGETS = approx-client approx-server
OBJDIR  = build

SRC_CLIENT = approx-client.c
SRC_SERVER = approx-server.c server_utils.c server_handle_messages.c
SRC_COMMON = err.c common.c

OBJ_CLIENT = $(SRC_CLIENT:%.c=$(OBJDIR)/%.o) $(SRC_COMMON:%.c=$(OBJDIR)/%.o)
OBJ_SERVER = $(SRC_SERVER:%.c=$(OBJDIR)/%.o) $(SRC_COMMON:%.c=$(OBJDIR)/%.o)

DEPS = $(OBJ_CLIENT:.o=.d) $(OBJ_SERVER:.o=.d)

all: $(OBJDIR) $(TARGETS)

approx-client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

approx-server: $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

-include $(DEPS)

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(TARGETS) *~