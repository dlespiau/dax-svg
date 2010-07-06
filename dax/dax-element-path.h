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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_ELEMENT_PATH_H__
#define __DAX_ELEMENT_PATH_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_PATH dax_element_path_get_type()

#define DAX_ELEMENT_PATH(obj)        \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 DAX_TYPE_ELEMENT_PATH, DaxElementPath))

#define DAX_ELEMENT_PATH_CLASS(klass)    \
    (G_TYPE_CHECK_CLASS_CAST ((klass),      \
                              DAX_TYPE_ELEMENT_PATH, DaxElementPathClass))

#define DAX_IS_ELEMENT_PATH(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_PATH))

#define DAX_IS_ELEMENT_PATH_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_PATH))

#define DAX_ELEMENT_PATH_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_PATH,   \
                                DaxElementPathClass))

typedef struct _DaxElementPath DaxElementPath;
typedef struct _DaxElementPathClass DaxElementPathClass;
typedef struct _DaxElementPathPrivate DaxElementPathPrivate;

struct _DaxElementPath
{
    DaxElement parent;

    DaxElementPathPrivate *priv;
};

struct _DaxElementPathClass
{
    DaxElementClass parent_class;
};

GType               dax_element_path_get_type       (void) G_GNUC_CONST;

DaxDomElement *     dax_element_path_new            (void);
ClutterPath2D *     dax_element_path_get_path       (DaxElementPath *self);
const DaxMatrix *   dax_element_path_get_transform  (DaxElementPath *path);

G_END_DECLS

#endif /* __DAX_ELEMENT_PATH_H__ */
