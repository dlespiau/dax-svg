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

#ifndef __CASTET_DOM_DOCUMENT_H__
#define __CASTET_DOM_DOCUMENT_H__

#include <glib-object.h>

#include "castet-dom-forward.h"
#include "castet-dom-node.h"
#include "castet-dom-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_DOM_DOCUMENT castet_dom_document_get_type()

#define CASTET_DOM_DOCUMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_DOM_DOCUMENT,  \
                                 CastetDomDocument))

#define CASTET_DOM_DOCUMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CASTET_TYPE_DOM_DOCUMENT, \
                              CastetDomDocumentClass))

#define CASTET_IS_DOM_DOCUMENT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_DOM_DOCUMENT))

#define CASTET_IS_DOM_DOCUMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              CASTET_TYPE_DOM_DOCUMENT))

#define CASTET_DOM_DOCUMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_DOM_DOCUMENT,   \
                                CastetDomDocumentClass))

typedef struct _CastetDomDocumentClass CastetDomDocumentClass;
typedef struct _CastetDomDocumentPrivate CastetDomDocumentPrivate;

struct _CastetDomDocument
{
    CastetDomNode parent;

    CastetDomDocumentPrivate *priv;
};

struct _CastetDomDocumentClass
{
    CastetDomNodeClass parent_class;

    CastetDomElement *(*create_element)    (CastetDomDocument  *self,
                                            const gchar        *tag_name,
                                            GError            **err);
    CastetDomElement *(*create_element_ns) (CastetDomDocument  *self,
                                            const gchar        *namespace_uri,
                                            const gchar        *qualified_name,
                                            GError            **err);
    CastetDomElement *(*get_element_by_id) (CastetDomDocument *self,
                                            const gchar       *id);
};

GType              castet_dom_document_get_type             (void) G_GNUC_CONST;

CastetDomDocument *castet_dom_document_new                  (void);

CastetDomElement  *castet_dom_document_get_document_element (CastetDomDocument *self);

CastetDomElement  *castet_dom_document_create_element       (CastetDomDocument  *self,
                                                             const gchar        *tag_name,
                                                             GError            **err);

CastetDomElement  *castet_dom_document_create_element_ns    (CastetDomDocument  *self,
                                                             const gchar        *namespace_uri,
                                                             const gchar        *qualified_name,
                                                             GError            **err);

CastetDomElement  *castet_dom_document_get_element_by_id    (CastetDomDocument *self,
                                                             const gchar       *id);

G_END_DECLS

#endif /* __CASTET_DOM_DOCUMENT_H__ */
