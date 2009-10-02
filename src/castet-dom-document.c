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
};

/*
 * CastetDomDocument
 */

CastetDomElement *
castet_dom_document_get_document_element(CastetDomDocument *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_DOCUMENT(self), NULL);

    return CASTET_DOM_ELEMENT(castet_dom_node_get_first_child(CASTET_DOM_NODE(self)));
}

/* DOM attributes */

/* DOM methods */

CastetDomElement *
castet_dom_document_create_element(CastetDomDocument  *self,
                                   const gchar        *tag_name,
                                   GError            **err)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->create_element)
        return klass->create_element(self, tag_name, err);

    return NULL;
}

CastetDomElement *
castet_dom_document_create_element_ns(CastetDomDocument  *self,
                                      const gchar        *namespace_uri,
                                      const gchar        *qualified_name,
                                      GError            **err)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->create_element_ns)
        return klass->create_element_ns(self, namespace_uri, qualified_name,
                                        err);

    return NULL;
}

CastetDomElement *
castet_dom_document_get_element_by_id(CastetDomDocument *self,
                                      const gchar       *id)
{
    CastetDomDocumentClass *klass = CASTET_DOM_DOCUMENT_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->get_element_by_id)
        return klass->get_element_by_id(self, id);

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
    self->priv = DOM_DOCUMENT_PRIVATE (self);
}

CastetDomDocument *
castet_dom_document_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_DOCUMENT, NULL);
}
