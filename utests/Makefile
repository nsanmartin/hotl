CFLAGS:=-g -std=c2x -Wall -Wextra -Werror -pedantic -Wold-style-definition -Wunused-function
SANITIZE_FLAGS:= -fsanitize=leak -fsanitize=address -fsanitize=undefined \
				 -fsanitize=null -fsanitize=bounds -fsanitize=pointer-overflow

BUILD=../build
OBJDIR=../build
SRCDIR=src
INCLUDE=../include
HEADERS=$(wildcard $(INCLUDE)/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRCS:src/%.c=$(OBJDIR)/%.o)

test_all: test_lipmap test_arl test_buf test_lipset
	$(BUILD)/test_lipmap \
		&& $(BUILD)/test_arl \
		&& $(BUILD)/test_buf \
		&& $(BUILD)/test_lipset


test_buf: test_buf.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@-E.c $<
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@ $^


test_lipmap: test_lipmap.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@-E.c $<
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@ $^


test_lipset: test_lipset.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@-E.c $<
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@ $^


test_arl: test_arl.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@-E.c $<
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I. -o $(BUILD)/$@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) \
		-I$(INCLUDE)    \
		-c -o $@     	\
		$< 

clean:
	find build -type f -delete
	if [ -f tags ]; then rm tags; fi

