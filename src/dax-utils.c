/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <math.h>
#include <stdlib.h>

#include "dax-utils.h"

void
_dax_utils_skip_space (char **str)
{
	while (g_ascii_isspace (**str))
		(*str)++;
}

void
_dax_utils_skip_space_and_char (char  **str,
                         gchar   skip_me)
{
	while (g_ascii_isspace (**str) || **str == skip_me)
		(*str)++;
}

gboolean
_dax_utils_parse_simple_float (gchar  **string,
                                  gfloat  *number)
{
    gchar *str = *string;
    gfloat value;

    if (!g_ascii_isdigit (*str))
        return FALSE;

    /* integer part */
    value = (gfloat) strtoul (str, (char **)&str, 10);

    if ((*str) == '.') {
        gfloat divisor = 0.1;

        /* 5. is not a valid number */
        if (!g_ascii_isdigit (*++str))
            return FALSE;

        while (g_ascii_isdigit (*str)) {
            value += (*str - '0') * divisor;
            divisor *= 0.1;
            str++;
        }
    }

    *string = str;
    *number = value;

    return TRUE;
}

gboolean
_dax_utils_parse_float (char   **string,
                           gfloat  *x)
{
	char *end, *c;
	gboolean integer_part = FALSE;
	gboolean fractional_part = FALSE;
	gboolean exponent_part = FALSE;
	gfloat mantissa = 0.0;
	gfloat exponent = 0.0;
	gfloat divisor;
	gboolean mantissa_sign = 1.0;
	gboolean exponent_sign = 1.0;

	c = *string;

	if (*c == '-') {
		mantissa_sign = -1.0;
		c++;
	} else if (*c == '+')
		c++;

	if (*c >= '0' && *c <= '9') {
		integer_part = TRUE;
		mantissa = *c - '0';
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa = mantissa * 10.0 + *c - '0';
			c++;
		}
	}


	if (*c == '.')
		c++;
	else if (!integer_part)
		return FALSE;

	if (*c >= '0' && *c <= '9') {
		fractional_part = TRUE;
		mantissa += (*c - '0') * 0.1;
		divisor = 0.01;
		c++;

		while (*c >= '0' && *c <= '9') {
			mantissa += (*c - '0') * divisor;
			divisor *= 0.1;
			c++;
		}
	}

	if (!fractional_part && !integer_part)
		return FALSE;

	end = c;

	if (*c == 'E' || *c == 'e') {
		c++;

		if (*c == '-') {
			exponent_sign = -1.0;
			c++;
		} else if (*c == '+')
			c++;

		if (*c >= '0' && *c <= '9') {
			exponent_part = TRUE;
			exponent = *c - '0';
			c++;

			while (*c >= '0' && *c <= '9') {
				exponent = exponent * 10.0 + *c - '0';
				c++;
			}
		}
	}

	if (exponent_part) {
		end = c;
		*x = mantissa_sign * mantissa * pow (10.0, exponent_sign * exponent);
	} else
		*x = mantissa_sign * mantissa;

	*string = end;

	return TRUE;
}