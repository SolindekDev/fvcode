#
# FVCode - Fast and easy code editor
# Copyright (C) 2023-2024 FVCode Developers
#   
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#   
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#   
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

CC = gcc
CFLAGS = -O3 -std=c2x -I./include $(shell pkg-config --cflags glfw3) -Wno-deprecated-declarations
LIBFLAGS = $(shell pkg-config --libs glfw3) -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

SRC_DIR = ./src
OBJ_DIR = ./obj

EXECUTABLE = fvcode

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(LIBFLAGS) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: clean
