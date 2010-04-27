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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_DOCUMENT_H__
#define __DAX_DOCUMENT_H__

#include <glib-object.h>

#include "dax-dom-document.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOCUMENT dax_document_get_type()

#define DAX_DOCUMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 DAX_TYPE_DOCUMENT,  \
                                 DaxDocument))

#define DAX_DOCUMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              DAX_TYPE_DOCUMENT, \
                              DaxDocumentClass))

#define DAX_IS_DOCUMENT(obj)         \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_DOCUMENT))

#define DAX_IS_DOCUMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),  \
                              DAX_TYPE_DOCUMENT))

#define DAX_DOCUMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_DOCUMENT,   \
                                DaxDocumentClass))

typedef struct _DaxDocument DaxDocument;
typedef struct _DaxDocumentClass DaxDocumentClass;
typedef struct _DaxDocumentPrivate DaxDocumentPrivate;

struct _DaxDocument
{
    DaxDomDocument parent;

    DaxDocumentPrivate *priv;
};

struct _DaxDocumentClass
{
    DaxDomDocumentClass parent_class;
};

GType dax_document_get_type (void) G_GNUC_CONST;

DaxDomDocument *    dax_document_new            (void);
const gchar *       dax_document_get_base_iri   (DaxDocument *document);
void                dax_document_set_base_iri   (DaxDocument *document,
                                                 const char  *base_iri);

G_END_DECLS

#endif /* __DAX_DOCUMENT_H__ */
