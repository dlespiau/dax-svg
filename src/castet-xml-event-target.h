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

#ifndef __CASTET_XML_EVENT_TARGET__
#define __CASTET_XML_EVENT_TARGET__

#include <glib-object.h>

#include "castet-xml-forward.h"
#include "castet-xml-event-listener.h"

G_BEGIN_DECLS

#define CASTET_TYPE_XML_EVENT_TARGET (castet_xml_event_target_get_type ())

#define CASTET_XML_EVENT_TARGET(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 CASTET_TYPE_XML_EVENT_TARGET,  \
                                 CastetXmlEventTarget))

#define CASTET_IS_XML_EVENT_TARGET(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_XML_EVENT_TARGET))

#define CASTET_XML_EVENT_TARGET_GET_IFACE(obj)                      \
    (G_TYPE_INSTANCE_GET_INTERFACE ((obj),                          \
                                    CASTET_TYPE_XML_EVENT_TARGET,   \
                                    CastetXmlEventTargetIface))

typedef struct _CastetXmlEventTargetIface CastetXmlEventTargetIface;

/**
 * CastetXmlEventTargetIface:
 *
 * Since: 0.2
 */
struct _CastetXmlEventTargetIface
{
    /*< private >*/
    GTypeInterface parent_iface;

    /*< public >*/
    void (*add_event_listener) (CastetXmlEventTarget   *event_target,
                                const gchar            *type,
                                CastetXmlEventListener *listener,
                                gboolean                use_capture);

    void (*remove_event_listener) (CastetXmlEventTarget   *event_target,
                                   const gchar            *type,
                                   CastetXmlEventListener *listener,
                                   gboolean                use_capture);
};

GType   castet_xml_event_target_get_type                (void) G_GNUC_CONST;

void    castet_xml_event_target_add_event_listener      (CastetXmlEventTarget   *target,
                                                         const gchar            *type,
                                                         CastetXmlEventListener *listener,
                                                         gboolean                use_capture);

void    castet_xml_event_target_remove_event_listener   (CastetXmlEventTarget   *target,
                                                         const gchar            *type,
                                                         CastetXmlEventListener *listener,
                                                         gboolean                use_capture);

G_END_DECLS

#endif /* __CASTET_XML_EVENT_TARGET__ */
