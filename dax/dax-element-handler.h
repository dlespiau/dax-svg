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

#ifndef __DAX_ELEMENT_HANDLER_H__
#define __DAX_ELEMENT_HANDLER_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_HANDLER dax_element_handler_get_type()

#define DAX_ELEMENT_HANDLER(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_ELEMENT_HANDLER,   \
                                 DaxElementHandler))

#define DAX_ELEMENT_HANDLER_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_ELEMENT_HANDLER,  \
                              DaxElementHandlerClass))

#define DAX_IS_ELEMENT_HANDLER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_HANDLER))

#define DAX_IS_ELEMENT_HANDLER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_HANDLER))

#define DAX_ELEMENT_HANDLER_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_ELEMENT_HANDLER,    \
                                DaxElementHandlerClass))

typedef struct _DaxElementHandler DaxElementHandler;
typedef struct _DaxElementHandlerClass DaxElementHandlerClass;
typedef struct _DaxElementHandlerPrivate DaxElementHandlerPrivate;

struct _DaxElementHandler
{
    DaxElement parent;

    DaxElementHandlerPrivate *priv;
};

struct _DaxElementHandlerClass
{
    DaxElementClass parent_class;
};

GType               dax_element_handler_get_type     (void) G_GNUC_CONST;

DaxDomElement *  dax_element_handler_new          (void);
DaxDomElement *  dax_element_handler_get_target   (DaxElementHandler *handler);
gchar *             dax_element_handler_get_code     (const DaxElementHandler *handler);

G_END_DECLS

#endif /* __DAX_ELEMENT_HANDLER_H__ */
