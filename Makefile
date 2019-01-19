# SPDX-License-Identifier: MIT
# Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

TMP=tmp

COMPILER=clang
# COMPILER=gcc

CROSS_COMPILE = 
ARCH_FLAGS = -march=native
OPTIM_FLAGS = -O3 -fno-math-errno # -ffast-math
DEBUG_FLAGS = #-g

INCLUDES = -I. -Iscicpp

WARNINGS = -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic
WARNINGS += -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion
WARNINGS += -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2
WARNINGS += -Wfloat-equal #-Weffc++

ifeq ($(COMPILER),gcc)
  CC = $(CROSS_COMPILE)gcc-8
  CCXX = $(CROSS_COMPILE)g++-8

  WARNINGS += -Wuseless-cast -Wlogical-op -Wduplicated-cond
else
  CC = $(CROSS_COMPILE)clang
  CCXX = $(CROSS_COMPILE)clang
  WARNINGS += -Wpessimizing-move -Wno-zero-length-array -Wextra-semi -Wreserved-id-macro
#   WARNINGS += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation-unknown-command -Wno-newline-eof -ferror-limit=200
endif

CFLAGS += -Werror $(WARNINGS) $(ARCH_FLAGS) $(OPTIM_FLAGS) $(DEBUG_FLAGS) $(INCLUDES) -MMD -MP
CXXFLAGS = $(CFLAGS) -std=c++17 -pthread
LD_FLAGS = -lm

ifeq ($(COMPILER),clang)
  # CXXFLAGS += -stdlib=libc++ # No C++17 deduction guides for now
  LD_FLAGS += -fuse-ld=gold -lstdc++
endif

all: test

.PHONY: setup_clang
setup_clang:
	apt-get install -y clang libc++-dev clang-format
	
$(TMP)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CCXX) -c $(CXXFLAGS) -o $@ $<
	
# -------------------------------------------------------------------------------------
# Tests
# -------------------------------------------------------------------------------------

TMP_TEST = $(TMP)/tests
TEST_TARGET = $(TMP_TEST)/test
CATCH_HPP = tests/catch.hpp
TEST_OBJ = $(TMP_TEST)/scicpp_test.o
TEST_MAIN_OBJ = $(TMP_TEST)/tests_main.o
TEST_DEP = $(subst .o,.d,$(TEST_OBJ))

-include $(TEST_DEP)

.PHONY: setup_test
setup_test: $(CATCH_HPP)

$(CATCH_HPP):
	curl -L https://github.com/catchorg/Catch2/releases/download/v2.0.1/catch.hpp -o $(CATCH_HPP)
	
$(TEST_TARGET): $(TEST_OBJ) $(TEST_MAIN_OBJ)
	$(CCXX) -o $@ $(TEST_OBJ) $(TEST_MAIN_OBJ) $(CXXFLAGS) $(LD_FLAGS) $(LIBS)

.PHONY: test
test: $(TEST_TARGET)
	$< -s -r compact

.PHONY: clean_test
clean_test:
	rm -rf $(TMP_TEST)

# -------------------------------------------------------------------------------------
# Benchmarks
# -------------------------------------------------------------------------------------

TMP_BCHMK = $(TMP)/benchmarks
BCHMK_TARGET = $(TMP_BCHMK)/benchmark
BCHMK_OBJ = $(TMP_BCHMK)/scicpp_benchmark.o
BCHMK_DEP = $(subst .o,.d,$(BCHMK_OBJ))

-include $(BCHMK_DEP)

BCHMK_CCXXFLAGS = $(ARCH_FLAGS) $(OPTIM_FLAGS) $(DEBUG_FLAGS) -std=c++17 -pthread -I. -Iscicpp -MMD -MP
# Don't compile Eigen runtime asserts
BCHMK_CCXXFLAGS += -DNDEBUG  #-funsafe-loop-optimizations

$(BCHMK_OBJ): benchmarks/scicpp_benchmark.cpp
	@mkdir -p $(dir $@)
	$(CCXX) -c $(BCHMK_CCXXFLAGS) -o $@ $<

$(BCHMK_TARGET): $(BCHMK_OBJ)
	$(CCXX) -o $@ $< $(BCHMK_CCXXFLAGS) $(LD_FLAGS) $(LIBS)

.PHONY: setup_benchmark
setup_benchmark:
	apt-get install libboost-all-dev

.PHONY: benchmark
benchmark: $(BCHMK_TARGET)
	$< -v -r html -o $(TMP_BCHMK)/results.html
	firefox $(TMP_BCHMK)/results.html

.PHONY: clean_benchmark
clean_benchmark:
	rm -rf $(TMP_BCHMK)

# -------------------------------------------------------------------------------------
# Examples
# -------------------------------------------------------------------------------------

EXAMPLE := white-noise
EXAMPLE_TARGET = $(TMP)/examples/$(EXAMPLE)
EXAMPLE_OBJ = $(TMP)/examples/$(EXAMPLE).o
EXAMPLE_PY = examples/$(notdir $(basename $(EXAMPLE_OBJ))).py
EXAMPLE_DEP = $(subst .o,.d,$(EXAMPLE_OBJ))

-include $(EXAMPLE_DEP)

$(EXAMPLE_TARGET): $(EXAMPLE_OBJ)
	$(CCXX) -o $@ $(EXAMPLE_OBJ) $(CXXFLAGS) $(LD_FLAGS) $(LIBS)

.PHONY: example
example: $(EXAMPLE_TARGET)
	@echo ======= C++ =======
	time -p $(EXAMPLE_TARGET)
	@echo
	@echo ======= Python =======
	time -p python $(EXAMPLE_PY)
	
.PHONY: clean
clean:
	rm -rf $(TMP)

# -------------------------------------------------------------------------------------
# Documentation
# -------------------------------------------------------------------------------------

.PHONY: setup_documentation
setup_documentation:
	apt-get install -y python-sphinx
	apt-get install -y doxygen
	pip install sphinx-autobuild exhale breathe
	pip install sphinx_bootstrap_theme

# -------------------------------------------------------------------------------------
# Tooling
# -------------------------------------------------------------------------------------

SCRIPT_TIDY_SH = scripts/tidy.sh
SCRIPT_FORMAT_SH = scripts/format.sh
SCRIPT_CPPCHECK_SH = scripts/cppcheck.sh

.PHONY: tidy
tidy: $(SCRIPT_TIDY_SH)
	bash $<

.PHONY: format
format: $(SCRIPT_FORMAT_SH)
	bash $<

.PHONY: cppcheck
cppcheck: $(SCRIPT_CPPCHECK_SH)
	bash $<