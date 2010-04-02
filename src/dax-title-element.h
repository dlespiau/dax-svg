/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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

#ifndef __DAX_TITLE_ELEMENT_H__
#define __DAX_TITLE_ELEMENT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_TITLE_ELEMENT dax_title_element_get_type()

#define DAX_TITLE_ELEMENT(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_TITLE_ELEMENT, \
                                 DaxTitleElement))

#define DAX_TITLE_ELEMENT_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_TITLE_ELEMENT,    \
                              DaxTitleElementClass))

#define DAX_IS_TITLE_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_TITLE_ELEMENT))

#define DAX_IS_TITLE_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_TITLE_ELEMENT))

#define DAX_TITLE_ELEMENT_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_TITLE_ELEMENT,  \
                                DaxTitleElementClass))

typedef struct _DaxTitleElement DaxTitleElement;
typedef struct _DaxTitleElementClass DaxTitleElementClass;
typedef struct _DaxTitleElementPrivate DaxTitleElementPrivate;

struct _DaxTitleElement
{
    DaxElement parent;

    DaxTitleElementPrivate *priv;
};

struct _DaxTitleElementClass
{
    DaxElementClass parent_class;
};

GType dax_title_element_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_title_element_new (void);

G_END_DECLS

#endif /* __DAX_TITLE_ELEMENT_H__ */
