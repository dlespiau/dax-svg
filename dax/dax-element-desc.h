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

#ifndef __DAX_ELEMENT_DESC_H__
#define __DAX_ELEMENT_DESC_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_DESC dax_element_desc_get_type()

#define DAX_ELEMENT_DESC(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ELEMENT_DESC, DaxElementDesc))

#define DAX_ELEMENT_DESC_CLASS(klass)			\
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_DESC,	\
                              DaxElementDescClass))

#define DAX_IS_ELEMENT_DESC(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_DESC))

#define DAX_IS_ELEMENT_DESC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_DESC))

#define DAX_ELEMENT_DESC_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_ELEMENT_DESC,	\
                                DaxElementDescClass))

typedef struct _DaxElementDesc DaxElementDesc;
typedef struct _DaxElementDescClass DaxElementDescClass;
typedef struct _DaxElementDescPrivate DaxElementDescPrivate;

struct _DaxElementDesc
{
    DaxElement parent;

    DaxElementDescPrivate *priv;
};

struct _DaxElementDescClass
{
    DaxElementClass parent_class;
};

GType		dax_element_desc_get_type   (void) G_GNUC_CONST;

DaxDomElement *	dax_element_desc_new	    (void);

G_END_DECLS

#endif /* __DAX_ELEMENT_DESC_H__ */
