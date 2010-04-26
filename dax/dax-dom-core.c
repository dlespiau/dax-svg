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

#include "dax-dom-private.h"
#include "dax-dom-core.h"

const gchar *xml_ns;
const gchar *xmlns_ns;
const gchar *xmlevents_ns;
const gchar *xlink_ns;

gboolean
dax_dom_init (gint     *argc,
              gchar  ***argv,
              GError   *error)
{
    xml_ns = I_(DAX_DOM_XML_NS_URI);
    xmlns_ns = I_(DAX_DOM_XMLNS_NS_URI);
    xmlevents_ns = I_(DAX_DOM_XML_EVENTS_NS_URI);
    xlink_ns = I_(DAX_DOM_XLINK_NS_URI);

    return TRUE;
}
