/*
 * Dax - Load and draw SVG
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

#include <glib.h>

#ifndef __DAX_DOM_CORE_H__
#define __DAX_DOM_CORE_H__

G_BEGIN_DECLS

#define DAX_DOM_XML_NS_URI          "http://www.w3.org/XML/1998/namespace"
#define DAX_DOM_XMLNS_NS_URI        "http://www.w3.org/2000/xmlns/"
#define DAX_DOM_XML_EVENTS_NS_URI   "http://www.w3.org/2001/xml-events"
#define DAX_DOM_XLINK_NS_URI        "http://www.w3.org/1999/xlink"

extern const gchar *xml_ns;
extern const gchar *xmlns_ns;
extern const gchar *xmlevents_ns;
extern const gchar *xlink_ns;

gboolean        dax_dom_init        (gint    *argc,
                                     gchar ***argv,
                                     GError  *error);

G_END_DECLS

#endif /* __DAX_DOM_CORE_H__ */
