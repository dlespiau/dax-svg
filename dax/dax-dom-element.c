/*
 * Dax - Load and draw SVG
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <gio/gio.h>

#include "dax-enum-types.h"
#include "dax-dom-core.h"
#include "dax-debug.h"
#include "dax-dom-private.h"
#include "dax-dom-document.h"
#include "dax-dom-element.h"
#include "dax-utils.h"

#include "dax-paramspec.h"      /* SEPDOM: paramspecs are too high? */

static void dax_xml_event_target_init (DaxXmlEventTargetIface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (DaxDomElement,
                                  dax_dom_element,
                                  DAX_TYPE_DOM_NODE,
                                  G_IMPLEMENT_INTERFACE (DAX_TYPE_XML_EVENT_TARGET,
                                                         dax_xml_event_target_init))

#define DOM_ELEMENT_PRIVATE(o)                              \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_DOM_ELEMENT, \
                                      DaxDomElementPrivate))

enum
{
    PROP_0,

    PROP_ID,
    PROP_BASE_IRI,
};

enum
{
    PARSED,
    LOADED,

    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

struct _DaxDomElementPrivate
{
    guint to_load;                  /* number of children not yet "loaded" */

    /* properties */
    gchar *id;
    gchar *base_iri;                /* xml:base value in the DOM tree */
    gchar *resolved_base_iri;       /* cache resolved xml:base */
};

/*
 * Private helpers
 */

#if 0
static const gchar *
dax_dom_element_get_attribute_foreign (DaxDomElement  *self,
                                       const gchar    *ns,
                                       const gchar    *name,
                                       GError        **err)
{
    return NULL;
}
#endif

static void
invalidate_resolved_iri (DaxDomElement *element)
{
    DaxDomElementPrivate *priv = element->priv;

    if (priv->resolved_base_iri == NULL)
        return;

    /* resolved_base_iri can be base_iri in the case base_iri is an [UI]RI */
    if (priv->base_iri != priv->resolved_base_iri)
        g_free (priv->resolved_base_iri);

    priv->resolved_base_iri = NULL;
}

/*
 * Fallback when we are trying to set an attribute that does not have
 * the same namespace as the DomElement itself
 * @ns: interned string
 */
static void
dax_dom_element_set_attribute_foreign (DaxDomElement  *self,
                                       const gchar    *namespace_uri,
                                       const gchar    *qualified_name,
                                       const gchar    *value,
                                       GError        **err)
{
    DaxDomNode *node = DAX_DOM_NODE (self);
    const gchar *local_name;

    local_name = strchr (qualified_name, ':');
    if (G_UNLIKELY (local_name == NULL))
        local_name = qualified_name;
    else
        local_name++;

    /* handle xmlns declarations */
    if (namespace_uri == xmlns_ns) {
        DAX_NOTE (NAMESPACE,
                     "adding namespace '%s' with prefix %s", value, local_name);
        _dax_dom_document_add_namespace (node->owner_document,
                                         local_name,
                                         value);
        return;
    }

    dax_dom_element_set_attribute (self, local_name, value, err);
}

static gboolean
is_loaded (DaxDomElement *element)
{
    return DAX_DOM_ELEMENT_FLAG_IS_SET (element, LOADED);
}

static void
on_child_loaded (DaxDomElement *child,
                 DaxDomElement *element)
{
    DaxDomElementPrivate *priv = element->priv;

    priv->to_load--;
    if (priv->to_load == 0) {
        /* "loaded" is sent from here when this element is loaded and is
         * waiting for one or more of its children to be loaded */
        g_signal_emit (element, signals[LOADED], 0, TRUE);
    }
}

void
_dax_dom_element_signal_parsed (DaxDomElement *element)
{
    DaxDomElementPrivate *priv = element->priv;
    DaxDomNode *cur;

    /* We default to being loaded, it's up to the implementation of "parsed"
     * to call set_loaded (element, FALSE); if it's not loaded yet */
    dax_dom_element_set_loaded (element, TRUE);

    /* This element is now parsed */
    g_signal_emit (element, signals[PARSED], 0);
    DAX_DOM_ELEMENT_SET_FLAG (element, PARSED);

    /* this element is ready if loaded and if its children elements are loaded
     * too. If we need to wait for elements to be ready, connect the to the
     * loaded signal of such elements */
    cur = ((DaxDomNode *) element)->first_child;
    for ( ; cur ; cur = cur->next_sibling) {
        if (!DAX_IS_DOM_ELEMENT (cur))
            continue;

        if (is_loaded ((DaxDomElement *)cur))
            continue;

        priv->to_load++;
        g_signal_connect (cur, "loaded", G_CALLBACK (on_child_loaded), element);
    }

    if (priv->to_load == 0) {
        g_signal_emit (element, signals[LOADED], 0, TRUE);
        return;
    }

    DAX_NOTE (LOADING, "%s is waiting for %d children to be loaded",
              G_OBJECT_TYPE_NAME (element), priv->to_load);
}

/*
 * DaxXmlEventTarget "implementation"
 */

static void
dax_xml_event_target_init (DaxXmlEventTargetIface *iface)
{
    /* we use the default implementation of XmlEventTarget */
}

/*
 * DaxDomElement default signals
 */

static void
dax_dom_element_loaded_real (DaxDomElement *element,
                             gboolean       loaded)
{
    if (loaded) {
        DAX_NOTE (LOADING, "%s is now loaded", G_OBJECT_TYPE_NAME (element));
        DAX_DOM_ELEMENT_SET_FLAG (element, LOADED);
    } else {
        DAX_NOTE (LOADING, "%s has been unloaded",
                  G_OBJECT_TYPE_NAME (element));
        DAX_DOM_ELEMENT_UNSET_FLAG (element, LOADED);
        g_signal_emit (element, signals[LOADED], 0, FALSE);
    }
}

/*
 * GObject overloading
 */

static void
dax_dom_element_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    DaxDomElement *element = (DaxDomElement *) object;
    DaxDomElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_ID:
        g_value_set_string (value, priv->id);
        break;
    case PROP_BASE_IRI:
        g_value_set_string (value, dax_dom_element_get_base_iri (element));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_dom_element_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    DaxDomElement *element = (DaxDomElement *) object;
    DaxDomElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_ID:
       dax_dom_element_set_id (element, g_value_get_string (value));
       break;
    case PROP_BASE_IRI:
        /* Remove cached base_iri if needed */
        invalidate_resolved_iri (element);
        /* Time to get a new iri */
        if (priv->base_iri)
            g_free (priv->base_iri);
        priv->base_iri = g_value_dup_string (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec); }
}

static void
dax_dom_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_element_parent_class)->dispose (object);
}

static void
dax_dom_element_finalize (GObject *object)
{
    DaxDomElement *element = DAX_DOM_ELEMENT (object);
    DaxDomElementPrivate *priv = element->priv;

    if (priv->id) {
        DaxDomDocument *document = ((DaxDomNode *) element)->owner_document;

        _dax_dom_document_unset_id (document, priv->id);
        g_free (priv->id);
    }

    invalidate_resolved_iri (element);
    g_free (priv->base_iri);

    G_OBJECT_CLASS (dax_dom_element_parent_class)->finalize (object);
}

static void
dax_dom_element_class_init (DaxDomElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomElementClass *element_class = DAX_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxDomElementPrivate));

    object_class->get_property = dax_dom_element_get_property;
    object_class->set_property = dax_dom_element_set_property;
    object_class->dispose = dax_dom_element_dispose;
    object_class->finalize = dax_dom_element_finalize;

    element_class->loaded = dax_dom_element_loaded_real;

    pspec = dax_param_spec_string ("id",
                                   "ID",
                                   "A unique identifier for the element",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_NONE,
                                   xml_ns);
    g_object_class_install_property (object_class, PROP_ID, pspec);

    pspec = dax_param_spec_string ("base",
                                   "Base IRI",
                                   "base IRI other than the base IRI of the "
                                   "document or external entity",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_NONE,
                                   xml_ns);
    g_object_class_install_property (object_class, PROP_BASE_IRI, pspec);

    signals[PARSED] =
        g_signal_new (I_("parsed"),
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (DaxDomElementClass, parsed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    signals[LOADED] =
        g_signal_new (I_("loaded"),
                      G_TYPE_FROM_CLASS (object_class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (DaxDomElementClass, loaded),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__BOOLEAN,
                      G_TYPE_NONE, 1,
                      G_TYPE_BOOLEAN);
}

static void
dax_dom_element_init (DaxDomElement *self)
{
    DaxDomElementPrivate *priv;

    self->priv = priv = DOM_ELEMENT_PRIVATE (self);

    priv->to_load = 1;      /* we need at least to load this element */
}

DaxDomElement *
dax_dom_element_new (void)
{
    return g_object_new (DAX_TYPE_DOM_ELEMENT, NULL);
}

/*
 * Accessors
 */

const gchar *
dax_dom_element_get_id (DaxDomElement *element)
{
    g_return_val_if_fail (DAX_DOM_ELEMENT (element), NULL);

    return element->priv->id;
}

void
dax_dom_element_set_id (DaxDomElement *element,
                        const gchar   *id)
{
    DaxDomElementPrivate *priv;
    DaxDomDocument *document;
    gboolean id_is_valid;
    gchar *new_id;

    g_return_if_fail (DAX_DOM_ELEMENT (element));

    priv = element->priv;

    new_id = g_strdup (id);
    document = ((DaxDomNode *) element)->owner_document;
    id_is_valid = _dax_dom_document_set_element_id (document, element, new_id);
    if (!id_is_valid) {
        g_free (new_id);
        return;
    }

    if (priv->id) {
        _dax_dom_document_unset_id (document, priv->id);
        g_free (priv->id);
    }

    priv->id = new_id;
    g_object_notify ((GObject *) element, "id");
}

void
dax_dom_element_set_loaded (DaxDomElement *element,
                            gboolean       loaded)
{
    DaxDomElementPrivate *priv = element->priv;

    g_return_if_fail (DAX_IS_DOM_ELEMENT (element));

    if (loaded && priv->to_load > 0) {
        priv->to_load--;
    } else {
        DAX_DOM_ELEMENT_UNSET_FLAG (element, LOADED);
        priv->to_load++;
    }

    if (priv->to_load == 0 && DAX_DOM_ELEMENT_FLAG_IS_SET (element, PARSED))
        g_signal_emit (element, signals[LOADED], 0, TRUE);
}

gboolean
dax_dom_element_is_loaded (DaxDomElement *element)
{
    g_return_val_if_fail (DAX_IS_DOM_ELEMENT (element), FALSE);

    return DAX_DOM_ELEMENT_FLAG_IS_SET (element, LOADED);
}

/*
 * Methods
 */

const gchar *
dax_dom_element_getAttributeNS (DaxDomElement  *self,
                                const gchar    *ns,
                                const gchar    *name,
                                GError        **err)
{
#if 0
    DaxDomElementClass *klass = DAX_DOM_ELEMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_attribute_NS)
        return klass->get_attribute_NS (self, ns, name, err);

#endif
    return NULL;
}

void
dax_dom_element_setAttributeNS (DaxDomElement  *self,
                                const gchar    *namespace_uri,
                                const gchar    *qualified_name,
                                const gchar    *value,
                                GError        **err)
{
    const gchar *local_name, *interned_ns;
    guint prefix_len = 0;

    interned_ns = g_intern_string (namespace_uri);

    /* search of a ASCII char in a UTF-8 string */
    local_name = strchr (qualified_name, ':');
    if (local_name) {
        gssize max = local_name - qualified_name;
        prefix_len = g_utf8_strlen (qualified_name, max);
    }

    /* the qualifiedName has a prefix and the namespaceURI is null */
    if (G_UNLIKELY (local_name != NULL && namespace_uri == NULL)) {
        g_warning ("NAMESPACE_ERR1");
        /* FIXME: raise NAMESPACE_ERR */
        return;
    }

    /* the qualifiedName has a prefix that is "xml" and the namespaceURI is
     * different from "http://www.w3.org/XML/1998/namespace" */
    if (G_UNLIKELY (local_name && strncmp (qualified_name, "xml:", 4) == 0 &&
                    interned_ns != xml_ns ))
    {
        g_warning ("NAMESPACE_ERR2");
        /* FIXME: raise NAMESPACE_ERR */
        return;
    }

    /* the qualifiedName or its prefix is "xmlns" and the namespaceURI is
     * different from "http://www.w3.org/2000/xmlns/" */
    if (G_UNLIKELY (interned_ns != xmlns_ns &&
                    ((local_name &&
                      strncmp (qualified_name, "xmlns:", 6) == 0) ||
                     strcmp (qualified_name, "xmlns") == 0)))
    {
        g_warning ("NAMESPACE_ERR3");
        /* FIXME: raise NAMESPACE_ERR */
        return;
    }

    /* the namespaceURI is "http://www.w3.org/2000/xmlns/" and neither the
     * qualifiedName nor its prefix is "xmlns" */
    if (G_UNLIKELY (interned_ns == xmlns_ns &&
                    (local_name &&
                      strncmp (qualified_name, "xmlns:", 6) != 0) &&
                     strcmp (qualified_name, "xmlns") == 0))
    {
        g_warning ("NAMESPACE_ERR4");
        /* FIXME: raise NAMESPACE_ERR */
        return;
    }

    if (interned_ns == NULL) {
        dax_dom_element_set_attribute (self, qualified_name, value, err);
        return;
    }

    if (interned_ns == DAX_DOM_NODE (self)->namespace_uri) {
        if (local_name == NULL)
            dax_dom_element_set_attribute (self, qualified_name, value, err);
        else
            dax_dom_element_set_attribute (self, local_name, value, err);
        return;
    }

    /* handle the rest, xmlns attributes, ev:event, ... */
    dax_dom_element_set_attribute_foreign (self,
                                           interned_ns,
                                           qualified_name,
                                           value,
                                           err);
}


const gchar *
dax_dom_element_getAttribute (DaxDomElement *self,
                              const gchar   *name)
{
    DaxDomElementClass *klass = DAX_DOM_ELEMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_attribute)
        return klass->get_attribute (self, name);

    return NULL;
}

void
dax_dom_element_setAttribute (DaxDomElement  *self,
                              const char     *name,
                              const char     *value,
                              GError        **err)
{
    DaxDomElementClass *klass = DAX_DOM_ELEMENT_GET_CLASS (self);

    g_return_if_fail (klass != NULL);

    if (klass->set_attribute)
        klass->set_attribute (self, name, value, err);
}

static const char *
get_parent_base_iri (DaxDomElement *element)
{
    DaxDomNode *parent;

    parent = ((DaxDomNode *) element)->parent_node;
    if (DAX_IS_DOM_DOCUMENT (parent))
        return dax_dom_document_get_base_iri ((DaxDomDocument *) parent);
    else
        return dax_dom_element_get_base_iri ((DaxDomElement *) parent);
}

const gchar *
dax_dom_element_get_base_iri (DaxDomElement *element)
{
    DaxDomElementPrivate *priv;

    g_return_val_if_fail (DAX_IS_DOM_ELEMENT (element), NULL);

    priv = element->priv;

    if (priv->resolved_base_iri)
        return priv->resolved_base_iri;

    /* If xml:base is set, it's time to do some work */
    if (priv->base_iri) {

        if (_dax_utils_is_iri (priv->base_iri)) {
            /* the value on the attribute is an actual uri */
            priv->resolved_base_iri = priv->base_iri;
        } else {
            /* relative uri to resolve */
            const gchar *parent_base_uri;
            GFile *resolved_file, *parent_file;

            parent_base_uri = get_parent_base_iri (element);
            parent_file = g_file_new_for_uri (parent_base_uri);

            resolved_file = g_file_resolve_relative_path (parent_file,
                                                          priv->base_iri);
            priv->resolved_base_iri = g_file_get_uri (resolved_file);

            g_object_unref (parent_file);
            g_object_unref (resolved_file);
        }

        return priv->resolved_base_iri;
    }

    return get_parent_base_iri (element);
}

