CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 
CXXFLAGS:=-g  -Wall -Wextra -Werror -pedantic 
SANITIZE_FLAGS:= -fsanitize=leak -fsanitize=address -fsanitize=undefined \
				 -fsanitize=null -fsanitize=bounds -fsanitize=pointer-overflow

BUILD:=../build
INCLUDE:=.

CHE_OBJDIR=../build
CHE_SRCDIR=./src
CHE_INCLUDE=../include
CHE_HEADERS=$(wildcard $(CHE_INCLUDE)/*.h)
CHE_SRCS=$(wildcard $(CHE_SRCDIR)/*.c)
CHE_OBJ=$(CHE_SRCS:src/%.c=$(CHE_OBJDIR)/%.o)

all: \
	collisions \
	count-words8 \
	rand-word8s \
	stdin-count-cppmap \
	stdin-count \
	stdin-count-cppmap \
	slot-sizes-words8


collisions: $(CHE_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

count-words8: $(CHE_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

rand-word8s: 
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $@.c 

stdin-count-cppmap: 
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$@ $@.c $^

stdin-count: $(CHE_OBJ)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

