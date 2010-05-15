/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#ifndef __DAX_ELEMENT_IMAGE_H__
#define __DAX_ELEMENT_IMAGE_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_IMAGE dax_element_image_get_type()

#define DAX_ELEMENT_IMAGE(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_IMAGE,    \
                                 DaxElementImage))

#define DAX_ELEMENT_IMAGE_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_IMAGE,   \
                              DaxElementImageClass))

#define DAX_IS_ELEMENT_IMAGE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_IMAGE))

#define DAX_IS_ELEMENT_IMAGE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_IMAGE))

#define DAX_ELEMENT_IMAGE_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_ELEMENT_IMAGE, \
                                DaxElementImageClass))

typedef struct _DaxElementImage DaxElementImage;
typedef struct _DaxElementImageClass DaxElementImageClass;
typedef struct _DaxElementImagePrivate DaxElementImagePrivate;

struct _DaxElementImage
{
    DaxElement parent;

    DaxElementImagePrivate *priv;
};

struct _DaxElementImageClass
{
    DaxElementClass parent_class;
};

GType                   dax_element_image_get_type          (void) G_GNUC_CONST;

DaxDomElement *         dax_element_image_new               (void);

ClutterUnits *          dax_element_image_get_x             (const DaxElementImage *image);
ClutterUnits *          dax_element_image_get_y             (const DaxElementImage *image);
ClutterUnits *          dax_element_image_get_width         (const DaxElementImage *image);
ClutterUnits *          dax_element_image_get_height        (const DaxElementImage *image);
const DaxCacheEntry *   dax_element_image_get_cache_entry   (const DaxElementImage *image);

G_END_DECLS

#endif /* __DAX_ELEMENT_IMAGE_H__ */
