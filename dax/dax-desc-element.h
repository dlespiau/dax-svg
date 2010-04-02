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

#ifndef __DAX_DESC_ELEMENT_H__
#define __DAX_DESC_ELEMENT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_DESC_ELEMENT dax_desc_element_get_type()

#define DAX_DESC_ELEMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_DESC_ELEMENT,  \
                                 DaxDescElement))

#define DAX_DESC_ELEMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_DESC_ELEMENT, \
                              DaxDescElementClass))

#define DAX_IS_DESC_ELEMENT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_DESC_ELEMENT))

#define DAX_IS_DESC_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_DESC_ELEMENT))

#define DAX_DESC_ELEMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_DESC_ELEMENT,   \
                                DaxDescElementClass))

typedef struct _DaxDescElement DaxDescElement;
typedef struct _DaxDescElementClass DaxDescElementClass;
typedef struct _DaxDescElementPrivate DaxDescElementPrivate;

struct _DaxDescElement
{
    DaxElement parent;

    DaxDescElementPrivate *priv;
};

struct _DaxDescElementClass
{
    DaxElementClass parent_class;
};

GType dax_desc_element_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_desc_element_new (void);

G_END_DECLS

#endif /* __DAX_DESC_ELEMENT_H__ */
