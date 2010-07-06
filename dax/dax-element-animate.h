/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#ifndef __DAX_ELEMENT_ANIMATE_H__
#define __DAX_ELEMENT_ANIMATE_H__

#include <glib-object.h>

#include "dax-element-animation.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_ANIMATE dax_element_animate_get_type()

#define DAX_ELEMENT_ANIMATE(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_ANIMATE,  \
                                 DaxElementAnimate))

#define DAX_ELEMENT_ANIMATE_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_ANIMATE,	\
                              DaxElementAnimateClass))

#define DAX_IS_ELEMENT_ANIMATE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_ANIMATE))

#define DAX_IS_ELEMENT_ANIMATE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_ANIMATE))

#define DAX_ELEMENT_ANIMATE_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_ANIMATE,   \
                                DaxElementAnimateClass))

typedef struct _DaxElementAnimate DaxElementAnimate;
typedef struct _DaxElementAnimateClass DaxElementAnimateClass;
typedef struct _DaxElementAnimatePrivate DaxElementAnimatePrivate;

struct _DaxElementAnimate
{
    DaxElementAnimation parent;

    DaxElementAnimatePrivate *priv;
};

struct _DaxElementAnimateClass
{
    DaxElementAnimationClass parent_class;
};

GType               dax_element_animate_get_type    (void) G_GNUC_CONST;

DaxDomElement *     dax_element_animate_new         (void);

G_END_DECLS

#endif /* __DAX_ELEMENT_ANIMATE_H__ */
