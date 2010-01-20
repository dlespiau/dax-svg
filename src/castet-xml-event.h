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

#ifndef __CASTET_XML_EVENT_H__
#define __CASTET_XML_EVENT_H__

#include <glib-object.h>

#include "castet-xml-forward.h"
#include "castet-xml-event-target.h"

G_BEGIN_DECLS

#define CASTET_TYPE_XML_EVENT           (castet_xml_event_get_type ())
#define CASTET_VALUE_HOLDS_XML_EVENT    (G_VALUE_HOLDS ((x), \
                                                        CASTET_TYPE_XML_EVENT))

typedef struct _CastetXmlMouseEvent CastetXmlMouseEvent;
typedef struct _CastetXmlAnyEvent   CastetXmlAnyEvent;

#define CASTET_XML_EVENT_TYPE_DEFAULT   CASTET_XML_EVENT_TYPE_NONE
typedef enum
{
    CASTET_XML_EVENT_TYPE_NONE,
    CASTET_XML_EVENT_TYPE_CLICK,
} CastetXmlEventType;

struct _CastetXmlAnyEvent
{
    CastetXmlEventType type;
    CastetXmlEventTarget *target;
    CastetXmlEventTarget *current_target;
    gboolean cancelable;
    gboolean default_prevented;
};

struct _CastetXmlMouseEvent
{
    CastetXmlEventType type;
    CastetXmlEventTarget *target;
    CastetXmlEventTarget *current_target;
    gboolean cancelable;
    gboolean default_prevented;

    gint32 screenX;
    gint32 screenY;
    gint32 clientX;
    gint32 clientY;
    guint32 button;
};

union _CastetXmlEvent
{
    CastetXmlEventType type;
    CastetXmlAnyEvent any;
    CastetXmlMouseEvent mouse_event;
};

GType                   castet_xml_event_get_type        (void) G_GNUC_CONST;

CastetXmlEvent *        castet_xml_event_copy            (const CastetXmlEvent *event);
void                    castet_xml_event_free            (CastetXmlEvent *event);
#if 0
gboolean                castet_xml_event_from_string     (CastetXmlEvent *event,
                                                          const gchar    *string);
gchar *                 castet_xml_event_to_string       (const CastetXmlEvent *event);

#endif

G_END_DECLS

#endif
