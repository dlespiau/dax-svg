/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#include "castet-enum-types.h"
#include "castet-xml-event-target.h"
#include "castet-dom-document.h"
#include "castet-dom-element.h"

static void castet_xml_event_target_init (CastetXmlEventTargetIface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (CastetDomElement,
                                  castet_dom_element,
                                  CASTET_TYPE_DOM_NODE,
                                  G_IMPLEMENT_INTERFACE (CASTET_TYPE_XML_EVENT_TARGET,
                                                         castet_xml_event_target_init))

#define DOM_ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_DOM_ELEMENT,  \
                                      CastetDomElementPrivate))

typedef struct _EventListener EventListener;

struct _EventListener
{
    CastetXmlEventListener *listener;
    gboolean use_capture;
};

struct _CastetDomElementPrivate
{
    GPtrArray *param_pools;
    GHashTable *listeners;          /* event => list of <EventListener>s */
};

/*
 * Private helpers
 */

void
_castet_dom_element_handle_event (CastetDomElement *element,
                                  CastetXmlEvent   *event)
{
    CastetDomElementPrivate *priv = element->priv;
    const gchar *event_name;
    GSList *listeners_list, *iter;

    event_name = castet_enum_to_string (CASTET_TYPE_XML_EVENT_TYPE,
                                        event->type);

    listeners_list = g_hash_table_lookup (priv->listeners, event_name);
    if (listeners_list == NULL) {
        g_warning (G_STRLOC ": Received event %s on %s, but no listeners "
                   "registered",
                   event_name,
                   G_OBJECT_TYPE_NAME (element));
        return;
    }

    for (iter = listeners_list; iter; iter = g_slist_next (iter)) {
        EventListener *el = iter->data;

        castet_xml_event_listener_handle_event (el->listener, event);
    }
}

/*
 * CastetXmlEventTarget implementation
 */

static void
event_listener_free (gpointer data)
{
    EventListener *el = data;

    g_object_unref (el->listener);
    g_slice_free (EventListener, el);
}

static void
castet_dom_element_add_event_listener (CastetXmlEventTarget   *target,
                                       const gchar            *type,
                                       CastetXmlEventListener *listener,
                                       gboolean                use_capture)
{
    CastetDomElement *self = CASTET_DOM_ELEMENT (target);
    CastetDomElementPrivate *priv = self->priv;
    EventListener *new_el;
    GSList *listeners_list;

    if (priv->listeners == NULL) {
        priv->listeners = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                 g_free,
                                                 event_listener_free);
    }

    new_el = g_slice_new (EventListener);
    new_el->listener = g_object_ref (listener);
    new_el->use_capture = use_capture;

    listeners_list = g_hash_table_lookup (priv->listeners, type);

    /* FIXME no need to g_strdup an existing key */
    listeners_list = g_slist_append (listeners_list, new_el);
    g_hash_table_insert (priv->listeners, g_strdup (type), listeners_list);
}

static void
castet_dom_element_remove_event_listener (CastetXmlEventTarget   *target,
                                          const gchar            *type,
                                          CastetXmlEventListener *listener,
                                          gboolean                use_capture)
{
    /* FIXME */
}

static void
castet_xml_event_target_init (CastetXmlEventTargetIface *iface)
{
    iface->add_event_listener = castet_dom_element_add_event_listener;
    iface->remove_event_listener = castet_dom_element_remove_event_listener;
}

/*
 * CastetDomElement implementation
 */

static const gchar *
castet_dom_element_get_attribute_NS_real (CastetDomElement  *self,
                                          const gchar       *ns,
                                          const gchar       *name,
                                          GError           **err)
{
    return NULL;
}

static void
castet_dom_element_set_attribute_NS_real (CastetDomElement  *self,
                                          const gchar       *ns,
                                          const gchar       *name,
                                          const gchar       *value,
                                          GError           **err)
{
#if 0
    CastetDomNode *node = CASTET_DOM_NODE (self);
    CastetDomDocument *document;

    document = castet_dom_node_get_owner_document (node);
#endif
    g_message ("Unsuported attribute %s (%s)", name, ns);
}

/*
 * GObject overloading
 */

static void
castet_dom_element_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_dom_element_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_dom_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_element_parent_class)->dispose (object);
}

static void
castet_dom_element_finalize (GObject *object)
{
    CastetDomElement *self = CASTET_DOM_ELEMENT (object);
    CastetDomElementPrivate *priv = self->priv;

    if (priv->listeners)
        g_hash_table_destroy (priv->listeners);

    G_OBJECT_CLASS (castet_dom_element_parent_class)->finalize (object);
}

static void
castet_dom_element_class_init (CastetDomElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetDomElementPrivate));

    object_class->get_property = castet_dom_element_get_property;
    object_class->set_property = castet_dom_element_set_property;
    object_class->dispose = castet_dom_element_dispose;
    object_class->finalize = castet_dom_element_finalize;

    klass->get_attribute_NS = castet_dom_element_get_attribute_NS_real;
    klass->set_attribute_NS = castet_dom_element_set_attribute_NS_real;
}

static void
castet_dom_element_init (CastetDomElement *self)
{
    CastetDomElementPrivate *priv;
    GParamSpecPool *pool;

    self->priv = priv = DOM_ELEMENT_PRIVATE (self);

    priv->param_pools = g_ptr_array_sized_new (5);

    /* FIXME declare xml:id */
    /* xml */
    pool = g_param_spec_pool_new (FALSE);
    g_ptr_array_add (priv->param_pools, pool);
}

CastetDomElement *
castet_dom_element_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_ELEMENT, NULL);
}

const gchar *
castet_dom_element_get_attribute_NS (CastetDomElement  *self,
                                     const gchar       *ns,
                                     const gchar       *name,
                                     GError           **err)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_attribute_NS)
        return klass->get_attribute_NS (self, ns, name, err);

    return NULL;
}

void
castet_dom_element_set_attribute_NS (CastetDomElement  *self,
                                     const gchar       *ns,
                                     const gchar       *name,
                                     const gchar       *value,
                                     GError           **err)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS (self);

    g_return_if_fail (klass != NULL);

    if (klass->set_attribute_NS)
        klass->set_attribute_NS (self, ns, name, value, err);
}


const gchar *
castet_dom_element_get_attribute (CastetDomElement *self,
                                  const gchar      *name)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_attribute)
        return klass->get_attribute (self, name);

    return NULL;
}

void
castet_dom_element_set_attribute (CastetDomElement  *self,
                                  const char        *name,
                                  const char        *value,
                                  GError           **err)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS (self);

    g_return_if_fail (klass != NULL);

    if (klass->set_attribute)
        klass->set_attribute (self, name, value, err);
}

