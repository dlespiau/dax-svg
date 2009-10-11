/*
 * Castet - Load and draw SVG
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

#include <stdlib.h>

#include "castet-utils.h"

gboolean
_castet_utils_parse_simple_float (gchar  **string,
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

