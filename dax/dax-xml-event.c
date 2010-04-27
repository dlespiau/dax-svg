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
#include "dax-xml-event.h"

DaxXmlEvent *
dax_xml_event_copy (const DaxXmlEvent *event)
{
    if (event == NULL)
        return NULL;

    return g_slice_dup (DaxXmlEvent, event);
}

void
dax_xml_event_free (DaxXmlEvent *event)
{
    if (event == NULL)
        return;

    if (event->any.target)
        g_object_unref (event->any.target);

    g_slice_free (DaxXmlEvent, event);
}

GType
dax_xml_event_get_type (void)
{
    static volatile gsize dax_xml_event_type__volatile = 0;

    if (g_once_init_enter (&dax_xml_event_type__volatile)) {
        GType dax_xml_event_type =
            g_boxed_type_register_static (I_("DaxXmlEvent"),
                                          (GBoxedCopyFunc)dax_xml_event_copy,
                                          (GBoxedFreeFunc)dax_xml_event_free);
        g_once_init_leave (&dax_xml_event_type__volatile,
                           dax_xml_event_type);
    }

    return dax_xml_event_type__volatile;
}

void
dax_xml_event_from_type (DaxXmlEvent       *xml_event,
                         DaxXmlEventType    type,
                         DaxXmlEventTarget *target)
{
    DaxXmlAnyEvent *any;

    any = &xml_event->any;
    any->type = type;
    any->target = g_object_ref (target);
}
