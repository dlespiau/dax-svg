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

#ifndef __DAX_HANDLER_ELEMENT_H__
#define __DAX_HANDLER_ELEMENT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_HANDLER_ELEMENT dax_handler_element_get_type()

#define DAX_HANDLER_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_HANDLER_ELEMENT,   \
                                 DaxHandlerElement))

#define DAX_HANDLER_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_HANDLER_ELEMENT,  \
                              DaxHandlerElementClass))

#define DAX_IS_HANDLER_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_HANDLER_ELEMENT))

#define DAX_IS_HANDLER_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_HANDLER_ELEMENT))

#define DAX_HANDLER_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_HANDLER_ELEMENT,    \
                                DaxHandlerElementClass))

typedef struct _DaxHandlerElement DaxHandlerElement;
typedef struct _DaxHandlerElementClass DaxHandlerElementClass;
typedef struct _DaxHandlerElementPrivate DaxHandlerElementPrivate;

struct _DaxHandlerElement
{
    DaxElement parent;

    DaxHandlerElementPrivate *priv;
};

struct _DaxHandlerElementClass
{
    DaxElementClass parent_class;
};

GType               dax_handler_element_get_type     (void) G_GNUC_CONST;

DaxDomElement *  dax_handler_element_new          (void);
DaxDomElement *  dax_handler_element_get_target   (DaxHandlerElement *handler);
gchar *             dax_handler_element_get_code     (const DaxHandlerElement *handler);

G_END_DECLS

#endif /* __DAX_HANDLER_ELEMENT_H__ */
