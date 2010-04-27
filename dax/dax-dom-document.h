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

#ifndef __DAX_DOM_DOCUMENT_H__
#define __DAX_DOM_DOCUMENT_H__

#include <glib-object.h>

#include "dax-dom-forward.h"
#include "dax-dom-node.h"
#include "dax-dom-element.h"
#include "dax-dom-text.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOM_DOCUMENT dax_dom_document_get_type()

#define DAX_DOM_DOCUMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_DOM_DOCUMENT,  \
                                 DaxDomDocument))

#define DAX_DOM_DOCUMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_DOM_DOCUMENT, \
                              DaxDomDocumentClass))

#define DAX_IS_DOM_DOCUMENT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_DOM_DOCUMENT))

#define DAX_IS_DOM_DOCUMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_DOM_DOCUMENT))

#define DAX_DOM_DOCUMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_DOM_DOCUMENT,   \
                                DaxDomDocumentClass))

typedef struct _DaxDomDocumentClass DaxDomDocumentClass;
typedef struct _DaxDomDocumentPrivate DaxDomDocumentPrivate;

struct _DaxDomDocument
{
    DaxDomNode parent;

    DaxDomDocumentPrivate *priv;
};

struct _DaxDomDocumentClass
{
    DaxDomNodeClass parent_class;

    DaxDomElement *(*create_element)    (DaxDomDocument  *self,
                                         const gchar     *tag_name,
                                         GError         **err);
    DaxDomElement *(*create_element_ns) (DaxDomDocument  *self,
                                         const gchar     *namespace_uri,
                                         const gchar     *qualified_name,
                                         GError         **err);
    DaxDomElement *(*get_element_by_id) (DaxDomDocument *self,
                                         const gchar       *id);
};

GType           dax_dom_document_get_type             (void) G_GNUC_CONST;

DaxDomDocument *dax_dom_document_new                  (void);

DaxDomElement  *dax_dom_document_get_document_element (DaxDomDocument *self);

DaxDomElement  *dax_dom_document_create_element       (DaxDomDocument  *self,
                                                       const gchar     *tag_name,
                                                       GError         **err);

DaxDomElement  *dax_dom_document_create_element_ns    (DaxDomDocument  *self,
                                                       const gchar     *namespace_uri,
                                                       const gchar     *qualified_name,
                                                       GError         **err);

DaxDomText     *dax_dom_document_create_text_node     (DaxDomDocument  *self,
                                                       const gchar     *data);

#define dax_dom_document_get_element_by_id \
    dax_dom_document_getElementById

DaxDomElement  *dax_dom_document_getElementById       (DaxDomDocument *self,
                                                       const gchar    *id);

G_END_DECLS

#endif /* __DAX_DOM_DOCUMENT_H__ */
