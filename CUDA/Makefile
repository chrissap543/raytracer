# Current type of implementation
IMPL = CUDA

# Python and script
PYTHON = python3.11
PPMTOJPG = ppmtojpg.py
FILTER_CSV = $(SCRIPT_DIR)/filter_csv.sh
PROFILING = $(SCRIPT_DIR)/profiling.sh
CONVERT = $(SCRIPT_DIR)/convert.sh

# CUDA directory
CUDA_DIR = /usr/local/cuda-12.2

# Compiler options
CC = g++
NVCC = nvcc -ccbin $(CC)
NVCC_FLAGS = -arch=sm_75

# CUDA library, include, and linking:
CUDA_LIB_DIR = -L$(CUDA_DIR)/lib64
CUDA_INC_DIR = -I$(CUDA_DIR)/include
CUDA_LINK_LIBS = -lcudart

# Project structure
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include
SCRIPT_DIR = ../scripts
PROF_DIR = ../profiling_data/$(IMPL)
CSV_DIR = ../profiling_data/$(IMPL)/csv
PPM_DIR = ../profiling_data/$(IMPL)/images/ppm
JPG_DIR = ../profiling_data/$(IMPL)/images/jpg

EXEC = ray-tracing
OBJS = $(OBJ_DIR)/main.o

# Compiling

all: $(EXEC)

$(EXEC) : $(OBJS)
	$(NVCC) $(NVCC_FLAGS) $(CUDA_INC_DIR) $(CUDA_LIB_DIR) $(CUDA_LINK_LIBS) -o $(EXEC) $(OBJS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.cu $(wildcard $(INC_DIR)/*.h) | $(OBJ_DIR)
	$(NVCC) $(NVCC_FLAGS) $(CUDA_INC_DIR) -I$(INC_DIR) -o $@ -c $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(PPM_DIR):
	mkdir -p $(PPM_DIR)

$(JPG_DIR):
	mkdir -p $(JPG_DIR)

output: output.jpg

output.ppm: $(EXEC)
	rm -f $@
	./$(EXEC) > $@

output.jpg: output.ppm
	rm -f $@
	$(PYTHON) $(PPMTOJPG) $<

filter_csv: $(SCRIPT_DIR)/profiling_input_parameters.sh $(SCRIPT_DIR)/filter_csv.sh $(SCRIPT_DIR)/filter_csv.py
	rm -f $(CSV_DIR)/filtered_*
	$(FILTER_CSV) $(CSV_DIR)

profiling: $(EXEC)
	rm -f $(CSV_DIR)/*
	rm -f $(PPM_DIR)/*
	rm -f $(JPG_DIR)/*
	$(PROFILING) $(CSV_DIR)/profiling_data.csv $(PPM_DIR)/image ./$(EXEC)

convert_ppm_to_jpg: $(CONVERT)
	rm -f $(JPG_DIR)/*
	$(CONVERT) $(PPM_DIR) $(JPG_DIR)

clean:
	rm -rf $(EXEC) $(OBJ_DIR)
