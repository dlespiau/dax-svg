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

#include "dax-dom-private.h"
#include "dax-dom-core.h"
#include "dax-dom-element.h"
#include "dax-paramspec.h"

#include "dax-dom-document.h"

G_DEFINE_ABSTRACT_TYPE (DaxDomDocument,
                        dax_dom_document,
                        DAX_TYPE_DOM_NODE);

#define DOM_DOCUMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_DOM_DOCUMENT,    \
                                      DaxDomDocumentPrivate))

enum
{
    PROP_0,

    PROP_BASE_IRI
};

struct _DaxDomDocumentPrivate
{
    GPtrArray *namespaces;
    GHashTable *id2element;
    gchar *base_iri;
};

/*
 * private helpers
 */

void
_dax_dom_document_add_namespace_static (DaxDomDocument *document,
                                        const gchar    *uri,
                                        const gchar    *prefix)
{
    DaxXmlNamespace *ns;

    ns = g_slice_new (DaxXmlNamespace);
    ns->uri = g_intern_static_string (uri);
    ns->prefix = g_intern_static_string (prefix);

    g_ptr_array_add (document->priv->namespaces, ns);
}

void
_dax_dom_document_add_namespace (DaxDomDocument *document,
                                 const gchar    *uri,
                                 const gchar    *prefix)
{
    DaxXmlNamespace *ns;

    ns = g_slice_new (DaxXmlNamespace);
    ns->uri = g_intern_string (uri);
    ns->prefix = g_intern_string (prefix);

    g_ptr_array_add (document->priv->namespaces, ns);
}

const gchar *
_dax_dom_document_get_prefix_for_interned_uri (DaxDomDocument *document,
                                               const gchar    *uri)
{
    DaxDomDocumentPrivate *priv = document->priv;
    guint i;

    /* NULL is  valid value for uri */
    if (uri == NULL)
        return NULL;

    for (i = 0; i < priv->namespaces->len; i++) {
        DaxXmlNamespace *ns = g_ptr_array_index (priv->namespaces, i);

        if (ns->uri == uri)
            return ns->prefix;
    }

    return NULL;
}

static void
_dax_dom_document_free_namespaces (DaxDomDocument *document)
{
    DaxDomDocumentPrivate *priv = document->priv;
    guint i;

    for (i = 0; i < priv->namespaces->len; i++)
        g_slice_free (DaxXmlNamespace, priv->namespaces->pdata[i]);
    g_ptr_array_free (priv->namespaces, TRUE);
}

gboolean
_dax_dom_document_set_element_id (DaxDomDocument *document,
                                  DaxDomElement  *element,
                                  const gchar    *id)
{
    DaxDomDocumentPrivate *priv = document->priv;
    DaxDomElement *found;

    found = g_hash_table_lookup (priv->id2element, id);
    if (found) {
        g_warning ("Trying to set the ID of a %s element to %s, but this "
                   "ID is already taken", G_OBJECT_TYPE_NAME (element), id);
        return FALSE;
    }
    /* it's ok to cast to gchar * here as we don't provide a destruction
     * function for the keys in priv->id2element and thus the hash table does
     * not touch the key, ever */
    g_hash_table_insert (priv->id2element, (gchar *)id, element);
    return TRUE;
}

void
_dax_dom_document_unset_id (DaxDomDocument *document,
                            const gchar    *id)
{
    g_hash_table_remove (document->priv->id2element, id);
}

/*
 * DaxDomDocument implementation
 */

static DaxDomElement *
dax_dom_document_get_element_by_id_real (DaxDomDocument *document,
                                         const gchar    *id)
{
    return g_hash_table_lookup (document->priv->id2element, id);
}

/*
 * GObject implementation
 */

static void
dax_dom_document_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    DaxDomDocument *document = (DaxDomDocument *) object;
    DaxDomDocumentPrivate *priv = document->priv;

    switch (property_id)
    {
    case PROP_BASE_IRI:
        g_value_set_string (value, priv->base_iri);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_dom_document_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    DaxDomDocument *document = (DaxDomDocument *) object;

    switch (property_id)
    {
    case PROP_BASE_IRI:
        dax_dom_document_set_base_iri (document, g_value_get_string (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_dom_document_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_document_parent_class)->dispose (object);
}

static void
dax_dom_document_finalize (GObject *object)
{
    DaxDomDocument *document = DAX_DOM_DOCUMENT (object);
    DaxDomDocumentPrivate *priv = document->priv;

    _dax_dom_document_free_namespaces (document);
    g_hash_table_unref (priv->id2element);
    g_free (priv->base_iri);

    G_OBJECT_CLASS (dax_dom_document_parent_class)->finalize (object);
}

static void
dax_dom_document_class_init (DaxDomDocumentClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomDocumentClass *document_class = DAX_DOM_DOCUMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxDomDocumentPrivate));

    object_class->get_property = dax_dom_document_get_property;
    object_class->set_property = dax_dom_document_set_property;
    object_class->dispose = dax_dom_document_dispose;
    object_class->finalize = dax_dom_document_finalize;

    document_class->get_element_by_id = dax_dom_document_get_element_by_id_real;

    pspec = dax_param_spec_string ("base",
                                   "Base IRI",
                                   "base IRI other than the base IRI of the "
                                   "document or external entity",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_NONE,
                                   xml_ns);
    g_object_class_install_property (object_class, PROP_BASE_IRI, pspec);
}

static void
dax_dom_document_init (DaxDomDocument *self)
{
    DaxDomDocumentPrivate *priv;

    self->priv = priv = DOM_DOCUMENT_PRIVATE (self);

    priv->namespaces = g_ptr_array_sized_new (5);
    _dax_dom_document_add_namespace_static (self, xml_ns, "xml");
    _dax_dom_document_add_namespace_static (self, xmlns_ns, "xmlns");

    priv->id2element = g_hash_table_new (g_str_hash, g_str_equal);
}

DaxDomDocument *
dax_dom_document_new (void)
{
    return g_object_new (DAX_TYPE_DOM_DOCUMENT, NULL);
}

/*
 * DaxDomDocument
 */

DaxDomElement *
dax_dom_document_get_document_element (DaxDomDocument *document)
{
    DaxDomNode *document_node;
    DaxDomNode *element_node;

    g_return_val_if_fail (DAX_IS_DOM_DOCUMENT (document), NULL);

    document_node = DAX_DOM_NODE (document);
    element_node = dax_dom_node_get_first_child (document_node);

    return DAX_DOM_ELEMENT (element_node);
}

DaxDomText *
dax_dom_document_create_text_node (DaxDomDocument *document,
                                   const gchar    *data)
{
    DaxDomNode *node;
    DaxDomText *text;

    text = dax_dom_text_new ();
    if (G_UNLIKELY (text == NULL))
        return NULL;

    dax_dom_character_data_set_data (DAX_DOM_CHARACTER_DATA (text), data);
    node = DAX_DOM_NODE (text);
    node->owner_document = document;

    return text;
}

/* DOM attributes */

/* DOM methods */

DaxDomElement *
dax_dom_document_create_element (DaxDomDocument  *document,
                                 const gchar     *tag_name,
                                 GError         **err)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (document);
    DaxDomNode *node;
    DaxDomElement *element = NULL;

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element)
        element = klass->create_element (document, tag_name, err);

    if (G_UNLIKELY (element == NULL))
        return NULL;

    node = DAX_DOM_NODE (element);
    node->owner_document = document;

    return element;
}

DaxDomElement *
dax_dom_document_create_element_ns (DaxDomDocument  *document,
                                    const gchar     *namespace_uri,
                                    const gchar     *qualified_name,
                                    GError         **err)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (document);
    DaxDomNode *node;
    DaxDomElement *element = NULL;

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element_ns)
        element = klass->create_element_ns (document,
                                            namespace_uri,
                                            qualified_name,
                                            err);

    if (G_UNLIKELY (element == NULL))
        return NULL;

    node = DAX_DOM_NODE (element);
    node->owner_document = document;

    return element;
}

DaxDomElement *
dax_dom_document_getElementById (DaxDomDocument *document,
                                 const gchar    *id)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (document);

    g_return_val_if_fail (klass != NULL, NULL);

    return klass->get_element_by_id (document, id);
}

const gchar *
dax_dom_document_get_base_iri (DaxDomDocument *document)
{
    g_return_val_if_fail (DAX_IS_DOM_DOCUMENT (document), NULL);

    return document->priv->base_iri;
}

void
dax_dom_document_set_base_iri (DaxDomDocument *document,
                               const char  *base_iri)
{
    DaxDomDocumentPrivate *priv;

    g_return_if_fail (DAX_IS_DOM_DOCUMENT (document));

    priv = document->priv;

    if (priv->base_iri)
        g_free (priv->base_iri);
    priv->base_iri = g_strdup (base_iri);
}
