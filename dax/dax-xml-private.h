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

#ifndef __DAX_XML_PRIVATE_H__
#define __DAX_XML_PRIVATE_H__

#include <glib.h>

G_BEGIN_DECLS

/*
 * A XML namespace.
 * The "const char *" elements of that structure must be interned strings
 */
typedef struct _DaxXmlNamespace {
    const char *uri;
    const char *prefix;
} DaxXmlNamespace;

G_END_DECLS

#endif /* __DAX_XML_PRIVATE_H__ */
