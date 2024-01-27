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

#ifndef __FV_MATH_H
#define __FV_MATH_H

#include <fv/fv.h>

#define FV_MIN(i, j)             \
    (((i) < (j)) ? (i) : (j))    \

#define FV_MAX(i, j)             \
    (((i) > (j)) ? (i) : (j))    \

#ifdef __MATH_LONG_DOUBLE_CONSTANTS
# define FV_E       M_El       
# define FV_LOG2    M_LOG2El   
# define FV_LOG10   M_LOG10El  
# define FV_LN2     M_LN2l     
# define FV_LN10    M_LN10l    
# define FV_PI      M_PIl      
# define FV_PI_2    M_PI_2l    
# define FV_PI_4    M_PI_4l    
# define FV_1_PI    M_1_PIl    
# define FV_2_PI    M_2_PIl    
# define FV_SQRTPI  M_2_SQRTPIl
# define FV_SQRT2   M_SQRT2l   
# define FV_SQRT1_2 M_SQRT1_2l 
#else
# define FV_E       M_E       
# define FV_LOG2    M_LOG2E   
# define FV_LOG10   M_LOG10E  
# define FV_LN2     M_LN2     
# define FV_LN10    M_LN10    
# define FV_PI      M_PI      
# define FV_PI_2    M_PI_2    
# define FV_PI_4    M_PI_4    
# define FV_1_PI    M_1_PI    
# define FV_2_PI    M_2_PI    
# define FV_SQRTPI  M_2_SQRTPI
# define FV_SQRT2   M_SQRT2   
# define FV_SQRT1_2 M_SQRT1_2 
#endif /* __MATH_LONG_DOUBLE_CONSTANTS */

#endif /* __FV_MATH_H */