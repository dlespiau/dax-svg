/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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
#include <clutter/clutter.h>

#include "castet-internals.h"
#include "castet-debug.h"
#include "castet-utils.h"

#define XML_NS_URI          "http://www.w3.org/XML/1998/namespace"
#define XMLNS_NS_URI        "http://www.w3.org/2000/xmlns/"
#define XML_EVENTS_NS_URI   "http://www.w3.org/2001/xml-events"
#define SVG_NS_URI          "http://www.w3.org/2000/svg"

const gchar *xml_ns;
const gchar *xmlns_ns;
const gchar *xmlevents_ns;
const gchar *svg_ns;

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
    clutter_init (argc, argv);

    xml_ns = ISS(XML_NS_URI);
    xmlns_ns = ISS(XMLNS_NS_URI);
    xmlevents_ns = ISS(XML_EVENTS_NS_URI);
    svg_ns = ISS(SVG_NS_URI);

#if 0
    g_value_register_transform_func (G_TYPE_FLOAT,
                                     G_TYPE_STRING,
                                     _transform_float_string);
#endif
    g_value_register_transform_func (G_TYPE_STRING,
                                     G_TYPE_FLOAT,
                                     _transform_string_float);
}
