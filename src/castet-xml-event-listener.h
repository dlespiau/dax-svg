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

#ifndef __CASTET_XML_EVENT_LISTENER__
#define __CASTET_XML_EVENT_LISTENER__

#include <glib-object.h>

#include "castet-xml-forward.h"
#include "castet-xml-event.h"

G_BEGIN_DECLS

#define CASTET_TYPE_XML_EVENT_LISTENER (castet_xml_event_listener_get_type ())

#define CASTET_XML_EVENT_LISTENER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                             \
                                 CASTET_TYPE_XML_EVENT_LISTENER,    \
                                 CastetXmlEventListener))

#define CASTET_IS_XML_EVENT_LISTENER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_XML_EVENT_LISTENER))

#define CASTET_XML_EVENT_LISTENER_GET_IFACE(obj)                    \
    (G_TYPE_INSTANCE_GET_INTERFACE ((obj),                          \
                                    CASTET_TYPE_XML_EVENT_LISTENER, \
                                    CastetXmlEventListenerIface))

typedef struct _CastetXmlEventListenerIface CastetXmlEventListenerIface;

/**
 * CastetXmlEventListenerIface:
 *
 * Since: 0.2
 */
struct _CastetXmlEventListenerIface
{
    /*< private >*/
    GTypeInterface parent_iface;

    /*< public >*/
    void (*handle_event) (CastetXmlEventListener *event_listener,
                          CastetXmlEvent            *event);
};

GType   castet_xml_event_listener_get_type      (void) G_GNUC_CONST;

void    castet_xml_event_listener_handle_event  (CastetXmlEventListener *listener,
                                                 CastetXmlEvent         *event);

G_END_DECLS

#endif /* __CASTET_XML_EVENT_LISTENER__ */
