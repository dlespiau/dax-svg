/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include "dax-dom.h"

#include "dax-enum-types.h"

#include "dax-traverser-load.h"

G_DEFINE_TYPE (DaxTraverserLoad, dax_traverser_load, DAX_TYPE_TRAVERSER)

/*
 * DaxTraverser implementation
 */

static void
on_load_event (DaxDomElement *element,
               gboolean       loaded,
               gpointer       user_data)
{
    DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (element);
    DaxXmlEvent load_event;

    dax_xml_event_from_type (&load_event, DAX_XML_EVENT_TYPE_LOAD, target);

    dax_xml_event_target_handle_event (target,
                                       dax_xml_event_copy (&load_event));
}

static void
dax_traverser_load_traverse_handler (DaxTraverser      *traverser,
                                     DaxElementHandler *node)
{
    DaxDomElement *target;
    DaxXmlEventTarget *event_target;
    DaxXmlEventListener *event_listener;
    DaxXmlEventType event_type;
    const gchar *event_name;

    g_object_get (node, "event", &event_type, NULL);
    if (event_type != DAX_XML_EVENT_TYPE_LOAD)
        return;

    event_name = dax_enum_to_string (DAX_TYPE_XML_EVENT_TYPE, event_type);
    target = dax_element_handler_get_target (node);
    event_target = DAX_XML_EVENT_TARGET (target);
    event_listener = DAX_XML_EVENT_LISTENER (node);

    dax_xml_event_target_add_event_listener (event_target,
                                             event_name,
                                             event_listener,
                                             FALSE);

    if (dax_dom_element_is_loaded (target)) {
        on_load_event (target, TRUE, NULL);
    } else  {
        g_signal_connect (target, "loaded",
                          G_CALLBACK (on_load_event), NULL);
    }
}

/*
 * GObject implementation
 */

static void
dax_traverser_load_class_init (DaxTraverserLoadClass *klass)
{
    DaxTraverserClass *traverser_class = DAX_TRAVERSER_CLASS (klass);

    traverser_class->traverse_handler = dax_traverser_load_traverse_handler;
}

static void
dax_traverser_load_init (DaxTraverserLoad *self)
{
}

DaxTraverser *
dax_traverser_load_new (DaxDomNode *root)
{
  return g_object_new (DAX_TYPE_TRAVERSER_LOAD,
                       "root", root,
                       NULL);
}
