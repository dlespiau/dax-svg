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

#include "dax-internals.h"
#include "dax-xml-event-listener.h"

GType
dax_xml_event_listener_get_type (void)
{
    static GType a_type = 0;

    if (G_UNLIKELY (a_type == 0)) {
        a_type =
            g_type_register_static_simple (G_TYPE_INTERFACE,
                                           I_("DaxXmlEventListener"),
                                           sizeof (DaxXmlEventListenerIface),
                                           NULL, 0, NULL, 0);
    }

    return a_type;
}

void
dax_xml_event_listener_handle_event (DaxXmlEventListener *listener,
                                        DaxXmlEvent         *event)
{
    DaxXmlEventListenerIface *iface;

    g_return_if_fail (DAX_IS_XML_EVENT_LISTENER (listener));
    g_return_if_fail (event != NULL);

    iface = DAX_XML_EVENT_LISTENER_GET_IFACE (listener);
    iface->handle_event (listener, event);
}
