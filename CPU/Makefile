# Current type of implementation
IMPL = CPU

# Python and script
PYTHON = python3.11
PPM_TO_JPG = $(SCRIPT_DIR)/ppmtojpg.py
FILTER_CSV = $(SCRIPT_DIR)/filter_csv.sh
PROFILING = $(SCRIPT_DIR)/profiling.sh
CONVERT = $(SCRIPT_DIR)/convert.sh

# Compiler options
CC = g++

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
	$(CC) -o $(EXEC) $(OBJS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.cc $(wildcard $(INC_DIR)/*.h) | $(OBJ_DIR)
	$(CC) -I$(INC_DIR) -o $@ -c $<

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
	$(PYTHON) $(PPM_TO_JPG) $<

filter_csv: $(SCRIPT_DIR)/profiling_input_parameters.sh $(SCRIPT_DIR)/filter_csv.sh $(SCRIPT_DIR)/filter_csv.py
	rm -f $(CSV_DIR)/filtered_*
	$(FILTER_CSV)

profiling: $(EXEC)
	rm -f $(CSV_DIR)/*
	rm -f $(PPM_DIR)/*
	rm -f $(JPG_DIR)/*
	$(PROFILING) $(CSV_DIR)/profiling_data.csv $(PPM_DIR)/image ./${OBJ_DIR}/$(EXEC)

convert_ppm_to_jpg: $(CONVERT) $(JPG_DIR) $(PPM_DIR)
	rm -f $(JPG_DIR)/*
	$(CONVERT) $(PPM_DIR) $(JPG_DIR)

clean:
	rm -rf $(EXEC) $(OBJ_DIR)
