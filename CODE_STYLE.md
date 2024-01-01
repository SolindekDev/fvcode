# Code Style in FVCode repository
## Declaring Functions
This is how to declare a function properly with given code style
```c
fv_gui_component_t*
FV_FindComponentByName(fv_gui_t* gui, char* name)
{
    /* code */
    return /* value */;
}
```
Every function name should start with `FV_` after that you add the function usage using `PascalCase` other known by `UpperCamelCase`. For example I want to declare a function that will apply code highlighting, name of this function should be
```c
FV_ApplyCodeHighlighting
```
## Calling functions
This is how to call a function. Normaly it's like this
```c
FV_FindComponentByName(gui, "file_list");
```
But if there is so many arguments that they don't fit in single line you should do it like this
```c
FV_FunctionThatTakesManyArguments(first_argument,
                                  second_argument,
                                  ...);
```
## Headers
```c
/*
 * FVCode - Fast and easy code editor
 * Copyright (C) 2023-2024 FVCode Developers
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __FV_FILENAME_H
#define __FV_FILENAME_H

#endif /* __FV_FILENAME_H */
```
## Copyright
Copyright comment should be put in every file at the top of it
```c
/*
 * FVCode - Fast and easy code editor
 * Copyright (C) 2023-2024 FVCode Developers
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
```
## Variables
Variables should be named in `snake_case` for example:
```c
int total_words = FV_GetTotalWordsFromFile(file);
```
## Structures
Structures should be named in `snake_case`. All names should start with `fv_` and end with `_t` for example:
```c
typedef struct __fv_example_struct_t {
    char* example_field;
    u64   example_field2;
} fv_example_struct_t;
```
Also structure field name should be aligned and be named in `snake_case`.