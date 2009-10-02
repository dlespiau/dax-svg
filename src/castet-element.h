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

#ifndef __CASTET_ELEMENT_H__
#define __CASTET_ELEMENT_H__

#include <glib-object.h>

#include "castet-dom-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_ELEMENT castet_element_get_type()

#define CASTET_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 CASTET_TYPE_ELEMENT,   \
                                 CastetElement))

#define CASTET_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              CASTET_TYPE_ELEMENT,  \
                              CastetElementClass))

#define CASTET_IS_ELEMENT(obj)          \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_ELEMENT))

#define CASTET_IS_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                              CASTET_TYPE_ELEMENT))

#define CASTET_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_ELEMENT,    \
                                CastetElementClass))

typedef struct _CastetElement CastetElement;
typedef struct _CastetElementClass CastetElementClass;
typedef struct _CastetElementPrivate CastetElementPrivate;

struct _CastetElement
{
    CastetDomElement parent;

    CastetElementPrivate *priv;
};

struct _CastetElementClass
{
    CastetDomElementClass parent_class;
};

GType castet_element_get_type (void) G_GNUC_CONST;

CastetElement *castet_element_new (void);

G_END_DECLS

#endif /* __CASTET_ELEMENT_H__ */
