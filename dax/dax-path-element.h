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

#ifndef __DAX_PATH_ELEMENT_H__
#define __DAX_PATH_ELEMENT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_PATH_ELEMENT dax_path_element_get_type()

#define DAX_PATH_ELEMENT(obj)        \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 DAX_TYPE_PATH_ELEMENT, DaxPathElement))

#define DAX_PATH_ELEMENT_CLASS(klass)    \
    (G_TYPE_CHECK_CLASS_CAST ((klass),      \
                              DAX_TYPE_PATH_ELEMENT, DaxPathElementClass))

#define DAX_IS_PATH_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_PATH_ELEMENT))

#define DAX_IS_PATH_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_PATH_ELEMENT))

#define DAX_PATH_ELEMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_PATH_ELEMENT,   \
                                DaxPathElementClass))

typedef struct _DaxPathElement DaxPathElement;
typedef struct _DaxPathElementClass DaxPathElementClass;
typedef struct _DaxPathElementPrivate DaxPathElementPrivate;

struct _DaxPathElement
{
    DaxElement parent;

    DaxPathElementPrivate *priv;
};

struct _DaxPathElementClass
{
    DaxElementClass parent_class;
};

GType               dax_path_element_get_type    (void) G_GNUC_CONST;

DaxDomElement   *dax_path_element_new         (void);
ClutterPath        *dax_path_element_get_path    (DaxPathElement *self);

G_END_DECLS

#endif /* __DAX_PATH_ELEMENT_H__ */
