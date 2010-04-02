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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "dax-dom-private.h"
#include "dax-dom-element.h"
#include "dax-dom-document.h"

G_DEFINE_ABSTRACT_TYPE (DaxDomDocument,
                        dax_dom_document,
                        DAX_TYPE_DOM_NODE);

#define DOM_DOCUMENT_PRIVATE(o) 					            \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), 			            \
                                      DAX_TYPE_DOM_DOCUMENT, \
                                      DaxDomDocumentPrivate))

struct _DaxDomDocumentPrivate
{
    GPtrArray *namespaces;
};

/*
 * private helpers
 */

void
_dax_dom_document_add_namespace_static (DaxDomDocument *document,
                                           const gchar       *uri,
                                           const gchar       *prefix)
{
    DaxXmlNamespace *ns;

    ns = g_slice_new (DaxXmlNamespace);
    ns->uri = g_intern_static_string (uri);
    ns->prefix = g_intern_static_string (prefix);

    g_ptr_array_add (document->priv->namespaces, ns);
}

void
_dax_dom_document_add_namespace (DaxDomDocument *document,
                                    const gchar       *uri,
                                    const gchar       *prefix)
{
    DaxXmlNamespace *ns;

    ns = g_slice_new (DaxXmlNamespace);
    ns->uri = g_intern_string (uri);
    ns->prefix = g_intern_string (prefix);

    g_ptr_array_add (document->priv->namespaces, ns);
}

const gchar *
_dax_dom_document_get_prefix_for_interned_uri (DaxDomDocument *document,
                                                  const gchar       *uri)
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

/*
 * DaxDomDocument
 */

DaxDomElement *
dax_dom_document_get_document_element(DaxDomDocument *self)
{
    DaxDomNode *document_node;
    DaxDomNode *element_node;

    g_return_val_if_fail (DAX_IS_DOM_DOCUMENT (self), NULL);

    document_node = DAX_DOM_NODE (self);
    element_node = dax_dom_node_get_first_child (document_node);

    return DAX_DOM_ELEMENT (element_node);
}

/* DOM attributes */

/* DOM methods */

DaxDomElement *
dax_dom_document_create_element (DaxDomDocument  *self,
                                    const gchar        *tag_name,
                                    GError            **err)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (self);
    DaxDomNode *node;
    DaxDomElement *element = NULL;

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element)
        element = klass->create_element (self, tag_name, err);

    if (G_UNLIKELY (element == NULL))
        return NULL;

    node = DAX_DOM_NODE (element);
    node->owner_document = self;

    return element;
}

DaxDomElement *
dax_dom_document_create_element_ns (DaxDomDocument  *self,
                                       const gchar        *namespace_uri,
                                       const gchar        *qualified_name,
                                       GError            **err)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (self);
    DaxDomNode *node;
    DaxDomElement *element = NULL;

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element_ns)
        element = klass->create_element_ns (self,
                                            namespace_uri,
                                            qualified_name,
                                            err);

    if (G_UNLIKELY (element == NULL))
        return NULL;

    node = DAX_DOM_NODE (element);
    node->owner_document = self;

    return element;
}

DaxDomElement *
dax_dom_document_get_element_by_id (DaxDomDocument *self,
                                       const gchar       *id)
{
    DaxDomDocumentClass *klass = DAX_DOM_DOCUMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_element_by_id)
        return klass->get_element_by_id (self, id);

    return NULL;
}

/*
 * GObject overloading
 */

static void
dax_dom_document_get_property (GObject    *object,
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
dax_dom_document_set_property (GObject      *object,
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
dax_dom_document_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_document_parent_class)->dispose (object);
}

static void
dax_dom_document_finalize (GObject *object)
{
    DaxDomDocument *self = DAX_DOM_DOCUMENT (object);

    _dax_dom_document_free_namespaces (self);

    G_OBJECT_CLASS (dax_dom_document_parent_class)->finalize (object);
}

static void
dax_dom_document_class_init (DaxDomDocumentClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxDomDocumentPrivate));

    object_class->get_property = dax_dom_document_get_property;
    object_class->set_property = dax_dom_document_set_property;
    object_class->dispose = dax_dom_document_dispose;
    object_class->finalize = dax_dom_document_finalize;
}

static void
dax_dom_document_init (DaxDomDocument *self)
{
    DaxDomDocumentPrivate *priv;

    self->priv = priv = DOM_DOCUMENT_PRIVATE (self);

    priv->namespaces = g_ptr_array_sized_new (5);
    _dax_dom_document_add_namespace_static (self, xml_ns, "xml");
    _dax_dom_document_add_namespace_static (self, xmlns_ns, "xmlns");
}

DaxDomDocument *
dax_dom_document_new (void)
{
    return g_object_new (DAX_TYPE_DOM_DOCUMENT, NULL);
}

DaxDomText *
dax_dom_document_create_text_node (DaxDomDocument  *self,
                                      const gchar        *data)
{
    DaxDomNode *node;
    DaxDomText *text;

    text = dax_dom_text_new ();
    if (G_UNLIKELY (text == NULL))
        return NULL;

    dax_dom_character_data_set_data (DAX_DOM_CHARACTER_DATA (text), data);
    node = DAX_DOM_NODE (text);
    node->owner_document = self;

    return text;
}