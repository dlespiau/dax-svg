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

#ifndef __CASTET_DOCUMENT_H__
#define __CASTET_DOCUMENT_H__

#include <glib-object.h>

#include "castet-dom-document.h"

G_BEGIN_DECLS

#define CASTET_TYPE_DOCUMENT castet_document_get_type()

#define CASTET_DOCUMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 CASTET_TYPE_DOCUMENT,  \
                                 CastetDocument))

#define CASTET_DOCUMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              CASTET_TYPE_DOCUMENT, \
                              CastetDocumentClass))

#define CASTET_IS_DOCUMENT(obj)         \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_DOCUMENT))

#define CASTET_IS_DOCUMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),  \
                              CASTET_TYPE_DOCUMENT))

#define CASTET_DOCUMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_DOCUMENT,   \
                                CastetDocumentClass))

typedef struct _CastetDocument CastetDocument;
typedef struct _CastetDocumentClass CastetDocumentClass;
typedef struct _CastetDocumentPrivate CastetDocumentPrivate;

struct _CastetDocument
{
    CastetDomDocument parent;

    CastetDocumentPrivate *priv;
};

struct _CastetDocumentClass
{
    CastetDomDocumentClass parent_class;
};

GType castet_document_get_type (void) G_GNUC_CONST;

CastetDomDocument *castet_document_new (void);

G_END_DECLS

#endif /* __CASTET_DOCUMENT_H__ */
