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


#include <fv/fv_main.h>
#include <fv/fv_app.h>

/* Main entry of FVCode*/
i32 
main(i32 argc, char** argv)
{
    fv_app_t* fv = FV_CreateApp(argc, argv);

    /* Initialize and run the app */ 
              fv->Init(fv);
    return    fv->Run(fv) ;
}
