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

#include "castet-dom-private.h"
#include "castet-dom-element.h"
#include "castet-dom-document.h"

G_DEFINE_ABSTRACT_TYPE (CastetDomDocument,
                        castet_dom_document,
                        CASTET_TYPE_DOM_NODE);

#define DOM_DOCUMENT_PRIVATE(o) 					            \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), 			            \
                                      CASTET_TYPE_DOM_DOCUMENT, \
                                      CastetDomDocumentPrivate))

struct _CastetDomDocumentPrivate
{
    GPtrArray *namespaces;
};

static gchar xml_ns[]        = "http://www.w3.org/XML/1998/namespace";
static gchar xmlns_ns[]      = "http://www.w3.org/2000/xmlns/";

/*
 * private helpers
 */

void
_castet_dom_document_add_namespace (CastetDomDocument *document,
                                    const gchar       *uri,
                                    const gchar       *prefix)
{
    CastetXmlNamespace *ns;

    ns = g_slice_new (CastetXmlNamespace);
    ns->uri = g_intern_static_string (uri);
    ns->prefix = g_intern_static_string (prefix);

    g_ptr_array_add (document->priv->namespaces, ns);
}

static void
_castet_dom_document_free_namespaces (CastetDomDocument *document)
{
    CastetDomDocumentPrivate *priv = document->priv;
    guint i;

    for (i = 0; i < priv->namespaces->len; i++)
        g_slice_free (CastetXmlNamespace, priv->namespaces->pdata[i]);
}

/*
 * CastetDomDocument
 */

CastetDomElement *
castet_dom_document_get_document_element(CastetDomDocument *self)
{
    CastetDomNode *document_node;
    CastetDomNode *element_node;

    g_return_val_if_fail (CASTET_IS_DOM_DOCUMENT (self), NULL);

    document_node = CASTET_DOM_NODE (self);
    element_node = castet_dom_node_get_first_child (document_node);

    return CASTET_DOM_ELEMENT (element_node);
}

/* DOM attributes */

/* DOM methods */

CastetDomElement *
castet_dom_document_create_element (CastetDomDocument  *self,
                                    const gchar        *tag_name,
                                    GError            **err)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element)
        return klass->create_element (self, tag_name, err);

    return NULL;
}

CastetDomElement *
castet_dom_document_create_element_ns (CastetDomDocument  *self,
                                       const gchar        *namespace_uri,
                                       const gchar        *qualified_name,
                                       GError            **err)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->create_element_ns)
        return klass->create_element_ns (self,
                                         namespace_uri,
                                         qualified_name,
                                         err);

    return NULL;
}

CastetDomElement *
castet_dom_document_get_element_by_id (CastetDomDocument *self,
                                       const gchar       *id)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS (self);

    g_return_val_if_fail (klass != NULL, NULL);

    if (klass->get_element_by_id)
        return klass->get_element_by_id (self, id);

    return NULL;
}

/*
 * GObject overloading
 */

static void
castet_dom_document_get_property (GObject    *object,
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
castet_dom_document_set_property (GObject      *object,
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
castet_dom_document_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_document_parent_class)->dispose (object);
}

static void
castet_dom_document_finalize (GObject *object)
{
    CastetDomDocument *self = CASTET_DOM_DOCUMENT (object);

    _castet_dom_document_free_namespaces (self);

    G_OBJECT_CLASS (castet_dom_document_parent_class)->finalize (object);
}

static void
castet_dom_document_class_init (CastetDomDocumentClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetDomDocumentPrivate));

    object_class->get_property = castet_dom_document_get_property;
    object_class->set_property = castet_dom_document_set_property;
    object_class->dispose = castet_dom_document_dispose;
    object_class->finalize = castet_dom_document_finalize;
}

static void
castet_dom_document_init (CastetDomDocument *self)
{
    CastetDomDocumentPrivate *priv;

    self->priv = priv = DOM_DOCUMENT_PRIVATE (self);

    priv->namespaces = g_ptr_array_sized_new (5);
    _castet_dom_document_add_namespace (self, xml_ns, "xml");
    _castet_dom_document_add_namespace (self, xmlns_ns, "xmlns");
}

CastetDomDocument *
castet_dom_document_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_DOCUMENT, NULL);
}

CastetDomText *
castet_dom_document_create_text_node (CastetDomDocument  *self,
                                      const gchar        *data)
{
    CastetDomText *text;

    text = castet_dom_text_new ();
    castet_dom_character_data_set_data (CASTET_DOM_CHARACTER_DATA (text), data);

    return text;
}
