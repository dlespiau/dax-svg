/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#include <glib-object.h>

#include "castet-debug.h"
#include "castet-utils.h"

static void
_transform_string_float (const GValue *src,
                         GValue       *dest)
{
    gfloat value = 0.0f;
    gchar *str = (gchar *)g_value_get_string (src);

    _castet_utils_parse_simple_float (&str, &value);

    g_value_set_float (dest, value);
}

void
castet_init (gint    *argc,
             gchar ***argv)
{
#ifdef CASTET_ENABLE_DEBUG
    _castet_debug_init ();
#endif

    g_type_init ();

#if 0
    g_value_register_transform_func (G_TYPE_FLOAT,
                                     G_TYPE_STRING,
                                     _transform_float_string);
#endif
    g_value_register_transform_func (G_TYPE_STRING,
                                     G_TYPE_FLOAT,
                                     _transform_string_float);
}
