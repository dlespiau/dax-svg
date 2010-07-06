/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#ifndef __DAX_ELEMENT_ANIMATION_H__
#define __DAX_ELEMENT_ANIMATION_H__

#include <glib-object.h>

#include "dax-element.h"
#include "dax-types.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_ANIMATION dax_element_animation_get_type()

#define DAX_ELEMENT_ANIMATION(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_ELEMENT_ANIMATION,    \
                                 DaxElementAnimation))

#define DAX_ELEMENT_ANIMATION_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_ELEMENT_ANIMATION,   \
                              DaxElementAnimationClass))

#define DAX_IS_ELEMENT_ANIMATION(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_ANIMATION))

#define DAX_IS_ELEMENT_ANIMATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_ANIMATION))

#define DAX_ELEMENT_ANIMATION_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_ANIMATION, \
                                DaxElementAnimationClass))

typedef struct _DaxElementAnimation DaxElementAnimation;
typedef struct _DaxElementAnimationClass DaxElementAnimationClass;
typedef struct _DaxElementAnimationPrivate DaxElementAnimationPrivate;

struct _DaxElementAnimation
{
    DaxElement parent;

    DaxElementAnimationPrivate *priv;
};

struct _DaxElementAnimationClass
{
    DaxElementClass parent_class;
};

GType                   dax_element_animation_get_type              (void) G_GNUC_CONST;

DaxDomElement *         dax_element_animation_new                   (void);
DaxDuration *           dax_element_animation_get_duration          (DaxElementAnimation *self);
const gchar *           dax_element_animation_get_attribute_name    (DaxElementAnimation *self);
const gchar *           dax_element_animation_get_from              (DaxElementAnimation *self);
const gchar *           dax_element_animation_get_to                (DaxElementAnimation *self);
const DaxRepeatCount *	dax_element_animation_get_repeat_count      (DaxElementAnimation *self);
DaxDomElement *         dax_element_animation_get_target            (DaxElementAnimation *self);

G_END_DECLS

#endif /* __DAX_ELEMENT_ANIMATION_H__ */
