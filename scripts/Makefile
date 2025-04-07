CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 
CXXFLAGS:=-g  -Wall -Wextra -Werror -pedantic 

BUILD:=../build
INCLUDE:=.

CHE_OBJDIR=../build
CHE_SRCDIR=../src
CHE_INCLUDE=../include
CHE_HEADERS=$(wildcard $(CHE_INCLUDE)/*.h)
CHE_SRCS=$(wildcard $(CHE_SRCDIR)/*.c)
CHE_OBJ=$(CHE_SRCS:src/%.c=$(CHE_OBJDIR)/%.o)

all: \
	collisions \
	count-words8 \
	rand-word8-count \
	rand-word8s \
	stdin-count-cppmap \
	stdin-count \
	stdin-count-cppmap \


collisions: $(CHE_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

count-words8: $(CHE_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

rand-word8-count: $(CHE_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

rand-word8s: 
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $@.c 

stdin-count-cppmap: 
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$@ $@.c $^

stdin-count: $(CHE_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(CHE_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

slot-sizes-words8: $(CHE_OBJ)
	$(CC) $(CFLAGS) \
		-I$(INCLUDE) -I$(CHE_INCLUDE) -o $(BUILD)/$@-E.c $@.c $^ -lm
	$(CC) $(CFLAGS) \
		-I$(INCLUDE) -I$(CHE_INCLUDE) -o $(BUILD)/$@ $@.c $^ -lm
