MODULES= test libraries/HttpServer

SOURCES ?= $(wildcard $(addsuffix /*.cpp, $(MODULES)))
OBJECTS := $(addsuffix .o, $(addprefix .build/, $(basename $(SOURCES))))
DEPFILES := $(subst .o,.dep, $(subst .build/,.deps/, $(OBJECTS)))
TESTCPPFLAGS = -D_TEST_ -Ilibraries/HttpServer -Itest -Iarduino
CPPDEPFLAGS = -MMD -MP -MF .deps/$(basename $<).dep
TEST_TARGET=test_HttpServer

.PHONY: all test clean

all: test


test: $(TEST_TARGET)
	./$(TEST_TARGET)

.build/%.o: %.cpp
	mkdir -p .deps/$(dir $<)
	mkdir -p .build/$(dir $<)
	$(COMPILE.cpp) $(TESTCPPFLAGS) $(CPPDEPFLAGS) -o $@ $<

$(TEST_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -lstdc++ -o $@

clean:
	@rm -rf .deps/ .build/ $(TEST_TARGET)

-include $(DEPFILES)
