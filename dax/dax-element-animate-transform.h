/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Øyvind Kolås <pippin@linux.intel.com>
 *              Damien Lespiau <damien.lespiau@intel.com>
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

#ifndef __DAX_ELEMENT_ANIMATE_TRANSFORM_H__
#define __DAX_ELEMENT_ANIMATE_TRANSFORM_H__

#include <glib-object.h>

#include "dax-element-animation.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM \
    dax_element_animate_transform_get_type()

#define DAX_ELEMENT_ANIMATE_TRANSFORM(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                 \
                                 DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM,    \
                                 DaxElementAnimateTransform))

#define DAX_ELEMENT_ANIMATE_TRANSFORM_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                              \
                              DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM,   \
                              DaxElementAnimateTransformClass))

#define DAX_IS_ELEMENT_ANIMATE_TRANSFORM(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM))

#define DAX_IS_ELEMENT_ANIMATE_TRANSFORM_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM))

#define DAX_ELEMENT_ANIMATE_TRANSFORM_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                              \
                                DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM, \
                                DaxElementAnimateTransformClass))

typedef struct _DaxElementAnimateTransform DaxElementAnimateTransform;
typedef struct _DaxElementAnimateTransformClass DaxElementAnimateTransformClass;
typedef struct _DaxElementAnimateTransformPrivate DaxElementAnimateTransformPrivate;

#define DAX_ANIMATE_TRANSFORM_TYPE_DEFAULT DAX_ANIMATE_TRANSFORM_TYPE_NONE

typedef enum _DaxAnimateTransformType
{
    DAX_ANIMATE_TRANSFORM_TYPE_NONE,
    DAX_ANIMATE_TRANSFORM_TYPE_TRANSLATE,
    DAX_ANIMATE_TRANSFORM_TYPE_SCALE,
    DAX_ANIMATE_TRANSFORM_TYPE_ROTATE,
    DAX_ANIMATE_TRANSFORM_TYPE_SKEW_X,          /*< nick=skewX >*/
    DAX_ANIMATE_TRANSFORM_TYPE_SKEW_Y           /*< nick=skewY >*/
} DaxAnimateTransformType;

struct _DaxElementAnimateTransform
{
    DaxElementAnimation parent;

    DaxElementAnimateTransformPrivate *priv;
};

struct _DaxElementAnimateTransformClass
{
    DaxElementAnimationClass parent_class;
};

GType                   dax_element_animate_transform_get_type          (void) G_GNUC_CONST;

DaxDomElement *         dax_element_animate_transform_new               (void);
DaxAnimateTransformType dax_element_animate_transform_get_matrix_type   (DaxElementAnimateTransform *at);

G_END_DECLS

#endif /* __DAX_ELEMENT_ANIMATE_TRANSFORM_H__ */
