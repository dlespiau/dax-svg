/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include <gio/gio.h>

#include "dax-utils.h"
#include "dax-dom-utils.h"

gchar *
dax_dom_utils_get_uri_for_href (DaxDomElement *element,
                                const gchar   *href)
{
    const gchar *base_iri;
    gchar *resolved_iri;
    GFile *base_file, *resolved_file;

    if (_dax_utils_is_iri (href)) {
        return g_strdup (href);
    } else {
        base_iri = dax_dom_element_get_base_iri (element);
        base_file = g_file_new_for_uri (base_iri);

        resolved_file = g_file_resolve_relative_path (base_file, href);
        resolved_iri = g_file_get_uri (resolved_file);

        g_object_unref (base_file);
        g_object_unref (resolved_file);

        return resolved_iri;
    }
}
