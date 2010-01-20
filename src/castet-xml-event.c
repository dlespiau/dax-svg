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
#include "castet-xml-event.h"

CastetXmlEvent *
castet_xml_event_copy (const CastetXmlEvent *event)
{
    if (event == NULL)
        return NULL;

    return g_slice_dup (CastetXmlEvent, event);
}

void
castet_xml_event_free (CastetXmlEvent *event)
{
    if (event == NULL)
        return;

    g_slice_free (CastetXmlEvent, event);
}

GType
castet_xml_event_get_type (void)
{
    static volatile gsize castet_xml_event_type__volatile = 0;

    if (g_once_init_enter (&castet_xml_event_type__volatile)) {
        GType castet_xml_event_type =
            g_boxed_type_register_static (ISS("CastetXmlEvent"),
                                          (GBoxedCopyFunc)castet_xml_event_copy,
                                          (GBoxedFreeFunc)castet_xml_event_free);
        g_once_init_leave (&castet_xml_event_type__volatile,
                           castet_xml_event_type);
    }

    return castet_xml_event_type__volatile;
}
