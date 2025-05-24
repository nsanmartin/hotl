CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 
CXXFLAGS:=-g  -Wall -Wextra -Werror -pedantic 
SANITIZE_FLAGS:= -fsanitize=leak -fsanitize=address -fsanitize=undefined \
				 -fsanitize=null -fsanitize=bounds -fsanitize=pointer-overflow

BUILD:=../build
INCLUDE:=.

HOTL_OBJDIR=../build
HOTL_SRCDIR=./src
HOTL_INCLUDE=../include
HOTL_HEADERS=$(wildcard $(HOTL_INCLUDE)/*.h)
HOTL_SRCS=$(wildcard $(HOTL_SRCDIR)/*.c)
HOTL_OBJ=$(HOTL_SRCS:src/%.c=$(HOTL_OBJDIR)/%.o)

all: \
	collisions \
	count-words8 \
	rand-word8s \
	stdin-count-cppmap \
	stdin-count \
	stdin-count-cppmap


collisions: $(HOTL_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(HOTL_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

count-words8: $(HOTL_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(HOTL_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

rand-word8s: 
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $@.c 

stdin-count-cppmap: 
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$@ $@.c $^

stdin-count: $(HOTL_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(HOTL_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

