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

#ifndef __DAX_XML_EVENT_LISTENER__
#define __DAX_XML_EVENT_LISTENER__

#include <glib-object.h>

#include "dax-xml-forward.h"
#include "dax-xml-event.h"

G_BEGIN_DECLS

#define DAX_TYPE_XML_EVENT_LISTENER (dax_xml_event_listener_get_type ())

#define DAX_XML_EVENT_LISTENER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                             \
                                 DAX_TYPE_XML_EVENT_LISTENER,    \
                                 DaxXmlEventListener))

#define DAX_IS_XML_EVENT_LISTENER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_XML_EVENT_LISTENER))

#define DAX_XML_EVENT_LISTENER_GET_IFACE(obj)                    \
    (G_TYPE_INSTANCE_GET_INTERFACE ((obj),                          \
                                    DAX_TYPE_XML_EVENT_LISTENER, \
                                    DaxXmlEventListenerIface))

typedef struct _DaxXmlEventListenerIface DaxXmlEventListenerIface;

/**
 * DaxXmlEventListenerIface:
 *
 * Since: 0.2
 */
struct _DaxXmlEventListenerIface
{
    /*< private >*/
    GTypeInterface parent_iface;

    /*< public >*/
    void (*handle_event) (DaxXmlEventListener *event_listener,
                          DaxXmlEvent            *event);
};

GType   dax_xml_event_listener_get_type      (void) G_GNUC_CONST;

void    dax_xml_event_listener_handle_event  (DaxXmlEventListener *listener,
                                                 DaxXmlEvent         *event);

G_END_DECLS

#endif /* __DAX_XML_EVENT_LISTENER__ */
