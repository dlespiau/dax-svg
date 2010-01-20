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

#include "castet-internals.h"
#include "castet-xml-event-target.h"

GType
castet_xml_event_target_get_type (void)
{
    static GType a_type = 0;

    if (G_UNLIKELY (a_type == 0))
        a_type =
            g_type_register_static_simple (G_TYPE_INTERFACE,
                                           ISS("CastetXmlEventTarget"),
                                           sizeof (CastetXmlEventTargetIface),
                                           NULL, 0, NULL, 0);

    return a_type;
}

void
castet_xml_event_target_add_event_listener (CastetXmlEventTarget   *target,
                                            const gchar            *type,
                                            CastetXmlEventListener *listener,
                                            gboolean                use_capture)
{
    CastetXmlEventTargetIface *iface;

    g_return_if_fail (CASTET_IS_XML_EVENT_TARGET (target));
    g_return_if_fail (type != NULL);
    g_return_if_fail (CASTET_IS_XML_EVENT_LISTENER (listener));

    iface = CASTET_XML_EVENT_TARGET_GET_IFACE (target);
    iface->add_event_listener (target, type, listener, use_capture);
}

void
castet_xml_event_target_remove_event_listener (CastetXmlEventTarget   *target,
                                               const gchar            *type,
                                               CastetXmlEventListener *listener,
                                               gboolean                use_capture)
{
    CastetXmlEventTargetIface *iface;

    g_return_if_fail (CASTET_IS_XML_EVENT_TARGET (target));
    g_return_if_fail (type != NULL);
    g_return_if_fail (CASTET_IS_XML_EVENT_LISTENER (listener));

    iface = CASTET_XML_EVENT_TARGET_GET_IFACE (target);
    iface->remove_event_listener (target, type, listener, use_capture);
}
