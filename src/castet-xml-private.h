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

#ifndef __CASTET_XML_PRIVATE_H__
#define __CASTET_XML_PRIVATE_H__

#include <glib.h>

G_BEGIN_DECLS

/*
 * A XML namespace.
 * The "const char *" elements of that structure must be interned strings
 */
typedef struct _CastetXmlNamespace {
    const char *uri;
    const char *prefix;
} CastetXmlNamespace;

G_END_DECLS

#endif /* __CASTET_XML_PRIVATE_H__ */
