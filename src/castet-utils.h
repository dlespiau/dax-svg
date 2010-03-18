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

#ifndef __CASTET_UTILS_H__
#define __CASTET_UTILS_H__

#include <glib.h>

G_BEGIN_DECLS

void        _castet_utils_skip_space                (char **str);
void        _castet_utils_skip_space_and_char       (char  **str,
                                                     gchar   skip_me);
gboolean    _castet_utils_parse_simple_float        (gchar **string,
                                                     gfloat *number);
gboolean    _castet_utils_parse_float               (char   **string,
                                                     gfloat  *x);

G_END_DECLS

#endif /* __CASTET_UTILS_H__ */
