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

#ifndef __DAX_DOM_UTILS_H__
#define __DAX_DOM_UTILS_H__

#include <glib.h>

#include "dax-dom-element.h"

G_BEGIN_DECLS

gchar *     dax_dom_utils_get_uri_for_href  (DaxDomElement *element,
                                             const gchar   *href);

G_END_DECLS

#endif /* __DAX_DOM_UTILS_H__ */
