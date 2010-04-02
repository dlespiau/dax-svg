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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_XML_EVENT_H__
#define __DAX_XML_EVENT_H__

#include <glib-object.h>

#include "dax-xml-forward.h"
#include "dax-xml-event-target.h"

G_BEGIN_DECLS

#define DAX_TYPE_XML_EVENT           (dax_xml_event_get_type ())
#define DAX_VALUE_HOLDS_XML_EVENT    (G_VALUE_HOLDS ((x), \
                                                        DAX_TYPE_XML_EVENT))

typedef struct _DaxXmlMouseEvent DaxXmlMouseEvent;
typedef struct _DaxXmlAnyEvent   DaxXmlAnyEvent;

#define DAX_XML_EVENT_TYPE_DEFAULT   DAX_XML_EVENT_TYPE_NONE
typedef enum
{
    DAX_XML_EVENT_TYPE_NONE,
    DAX_XML_EVENT_TYPE_CLICK,
} DaxXmlEventType;

struct _DaxXmlAnyEvent
{
    DaxXmlEventType type;
    DaxXmlEventTarget *target;
    DaxXmlEventTarget *current_target;
    gboolean cancelable;
    gboolean default_prevented;
};

struct _DaxXmlMouseEvent
{
    DaxXmlEventType type;
    DaxXmlEventTarget *target;
    DaxXmlEventTarget *current_target;
    gboolean cancelable;
    gboolean default_prevented;

    gint32 screenX;
    gint32 screenY;
    gint32 clientX;
    gint32 clientY;
    guint32 button;
};

union _DaxXmlEvent
{
    DaxXmlEventType type;
    DaxXmlAnyEvent any;
    DaxXmlMouseEvent mouse_event;
};

GType                   dax_xml_event_get_type        (void) G_GNUC_CONST;

DaxXmlEvent *        dax_xml_event_copy            (const DaxXmlEvent *event);
void                    dax_xml_event_free            (DaxXmlEvent *event);
#if 0
gboolean                dax_xml_event_from_string     (DaxXmlEvent *event,
                                                          const gchar    *string);
gchar *                 dax_xml_event_to_string       (const DaxXmlEvent *event);

#endif

G_END_DECLS

#endif
