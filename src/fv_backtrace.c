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

#include <fv/fv.h>

/* Backtrace is avaliable only for linux machines */
#ifdef __unix__

#include <fv/fv_backtrace.h>
#include <execinfo.h>

/* More about backtraces in C:
 *  - https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
 */

#define BACKTRACE_SIZE 15

void 
FV_PrintBacktraceOfProgram()
{
    void* backtrace_array[BACKTRACE_SIZE];

    i32 backtrace_size          = backtrace(backtrace_array, BACKTRACE_SIZE);
    char** backtrace_symbol_str = backtrace_symbols(backtrace_array, backtrace_size);

    if (backtrace_symbol_str != NULL)
    {
        printf("\e[0;31mFVCode\e[0m: Obtained %d stack frames:\n", backtrace_size);
        for (i32 i = 0; i < backtrace_size; i++)
            printf(" - [\e[0;34m%d\e[0m] %s\n", i + 1, backtrace_symbol_str[i]);
    }

    free(backtrace_symbol_str);
    return;
}

#endif /* __unix__ */
