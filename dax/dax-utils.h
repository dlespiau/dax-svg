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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DAX_UTILS_H__
#define __DAX_UTILS_H__

#include <glib.h>
#include <clutter/clutter.h>

G_BEGIN_DECLS

void        _dax_utils_skip_space               (char **str);
void        _dax_utils_skip_space_and_char      (char  **str,
                                                 gchar   skip_me);
void        _dax_utils_find_next_space          (gchar **str);
guint       _dax_utils_count_commas             (const gchar *str);
guint       _dax_utils_count_words              (const gchar *str);
gboolean    _dax_utils_parse_simple_float       (gchar **string,
                                                 gfloat *number);
gboolean    _dax_utils_parse_float              (char   **string,
                                                 gfloat  *x);
gboolean    _dax_utils_is_iri                   (const gchar *str);
void        _dax_utils_dump_path_2d             (ClutterPath2D *path);
void        _dax_utils_dump_cogl_matrix         (CoglMatrix *m);

typedef enum {
    _DAX_PROP_TRANSFORM = 1,
} _DaxProp;

void        _dax_utils_install_properties       (GObjectClass *klass,
                                                 _DaxProp      first_property,
                                                 ...);

G_END_DECLS

#endif /* __DAX_UTILS_H__ */
