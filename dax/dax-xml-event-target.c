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

#include "dax-debug.h"
#include "dax-enum-types.h"
#include "dax-internals.h"

#include "dax-xml-event-target.h"

/*
 * DaxXmlEventTarget default implementation
 */

typedef struct _XmlEventContext XmlEventContext;

typedef struct _EventListener EventListener;

struct _EventListener
{
    DaxXmlEventListener *listener;
    gboolean use_capture;
};

struct _XmlEventContext
{
    GHashTable *listeners;          /* event => list of <EventListener>s */
};

static GQuark quark_event_target_context;

static void
event_listener_free (gpointer data)
{
    EventListener *el = data;

    g_object_unref (el->listener);
    g_slice_free (EventListener, el);
}

static void
xml_event_target_context_free (gpointer data)
{
    XmlEventContext *ctx = data;

    if (ctx->listeners)
        g_hash_table_destroy (ctx->listeners);
    g_slice_free (XmlEventContext, ctx);
}

static XmlEventContext *
xml_event_target_context_create (DaxXmlEventTarget *target)
{
    XmlEventContext *ctx;

    ctx = g_slice_new (XmlEventContext);

    ctx->listeners = g_hash_table_new_full (g_str_hash, g_str_equal,
                                            g_free,
                                            event_listener_free);

    g_object_set_qdata_full (G_OBJECT (target), quark_event_target_context,
                             ctx,
                             xml_event_target_context_free);
    return ctx;
}

static void
add_event_listener_default (DaxXmlEventTarget   *target,
                            const gchar         *type,
                            DaxXmlEventListener *listener,
                            gboolean             use_capture)
{
    XmlEventContext *ctx;
    EventListener *new_el;
    GSList *listeners_list;

    DAX_NOTE (EVENT, "add listener (%s) on %s for event \"%s\"",
              G_OBJECT_TYPE_NAME (listener), G_OBJECT_TYPE_NAME (target), type);

    ctx = g_object_get_qdata (G_OBJECT (target), quark_event_target_context);

    if (ctx == NULL)
        ctx = xml_event_target_context_create (target);

    new_el = g_slice_new (EventListener);
    new_el->listener = g_object_ref (listener);
    new_el->use_capture = use_capture;

    listeners_list = g_hash_table_lookup (ctx->listeners, type);

    /* FIXME no need to g_strdup an existing key */
    listeners_list = g_slist_append (listeners_list, new_el);
    g_hash_table_insert (ctx->listeners, g_strdup (type), listeners_list);
}

static void
remove_event_listener_default (DaxXmlEventTarget   *target,
                               const gchar         *type,
                               DaxXmlEventListener *listener,
                               gboolean             use_capture)
{
    /* FIXME */
}

static void
handle_event_default (DaxXmlEventTarget *target,
                      DaxXmlEvent       *event)
{
    GSList *listeners_list, *iter;
    const gchar *event_name;
    XmlEventContext *ctx;

    ctx = g_object_get_qdata (G_OBJECT (target), quark_event_target_context);
    if (G_UNLIKELY (ctx == NULL))
        return;

    event_name = dax_enum_to_string (DAX_TYPE_XML_EVENT_TYPE, event->type);

    listeners_list = g_hash_table_lookup (ctx->listeners, event_name);
    if (listeners_list == NULL) {
        g_warning (G_STRLOC ": Received event %s (%d) on %s, but no listeners "
                   "registered", event_name, event->type,
                   G_OBJECT_TYPE_NAME (target));
        return;
    }

    for (iter = listeners_list; iter; iter = g_slist_next (iter)) {
        EventListener *el = iter->data;

        DAX_NOTE (EVENT, "%s fires the \"%s\" event on %s",
                  G_OBJECT_TYPE_NAME (target), event_name,
                  G_OBJECT_TYPE_NAME(el->listener));

        dax_xml_event_listener_handle_event (el->listener, event);
    }
}

static void
dax_xml_event_target_base_init (gpointer g_iface)
{
    DaxXmlEventTargetIface *iface = g_iface;
    static gboolean is_initialized = FALSE;

    if (G_UNLIKELY (is_initialized))
        return;

    is_initialized = TRUE;

    quark_event_target_context =
        g_quark_from_static_string ("dax-xml-event-target-context");

    iface->add_event_listener = add_event_listener_default;
    iface->remove_event_listener = remove_event_listener_default;
    iface->handle_event = handle_event_default;
}

GType
dax_xml_event_target_get_type (void)
{
    static GType a_type = 0;

    if (G_UNLIKELY (a_type == 0)) {
        const GTypeInfo event_target_info = {
            sizeof (DaxXmlEventTargetIface),
            dax_xml_event_target_base_init,
            NULL, /* base_finalize */
        };

        a_type = g_type_register_static (G_TYPE_INTERFACE,
                                         I_("DaxXmlEventTarget"),
                                         &event_target_info, 0);
    }

    return a_type;
}

void
dax_xml_event_target_add_event_listener (DaxXmlEventTarget   *target,
                                         const gchar         *type,
                                         DaxXmlEventListener *listener,
                                         gboolean             use_capture)
{
    DaxXmlEventTargetIface *iface;

    g_return_if_fail (DAX_IS_XML_EVENT_TARGET (target));
    g_return_if_fail (type != NULL);
    g_return_if_fail (DAX_IS_XML_EVENT_LISTENER (listener));

    iface = DAX_XML_EVENT_TARGET_GET_IFACE (target);
    iface->add_event_listener (target, type, listener, use_capture);
}

void
dax_xml_event_target_remove_event_listener (DaxXmlEventTarget   *target,
                                            const gchar         *type,
                                            DaxXmlEventListener *listener,
                                            gboolean             use_capture)
{
    DaxXmlEventTargetIface *iface;

    g_return_if_fail (DAX_IS_XML_EVENT_TARGET (target));
    g_return_if_fail (type != NULL);
    g_return_if_fail (DAX_IS_XML_EVENT_LISTENER (listener));

    iface = DAX_XML_EVENT_TARGET_GET_IFACE (target);
    iface->remove_event_listener (target, type, listener, use_capture);
}

void
dax_xml_event_target_handle_event (DaxXmlEventTarget *target,
                                   DaxXmlEvent       *event)
{
    DaxXmlEventTargetIface *iface;

    g_return_if_fail (DAX_IS_XML_EVENT_TARGET (target));

    iface = DAX_XML_EVENT_TARGET_GET_IFACE (target);
    iface->handle_event (target, event);
}
