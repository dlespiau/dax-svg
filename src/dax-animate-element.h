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

#ifndef __DAX_ANIMATE_ELEMENT_H__
#define __DAX_ANIMATE_ELEMENT_H__

#include <glib-object.h>

#include "dax-element.h"
#include "dax-types.h"

G_BEGIN_DECLS

#define DAX_TYPE_ANIMATE_ELEMENT dax_animate_element_get_type()

#define DAX_ANIMATE_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_ANIMATE_ELEMENT,   \
                                 DaxAnimateElement))

#define DAX_ANIMATE_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_ANIMATE_ELEMENT,  \
                              DaxAnimateElementClass))

#define DAX_IS_ANIMATE_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ANIMATE_ELEMENT))

#define DAX_IS_ANIMATE_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ANIMATE_ELEMENT))

#define DAX_ANIMATE_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_ANIMATE_ELEMENT,    \
                                DaxAnimateElementClass))

typedef struct _DaxAnimateElement DaxAnimateElement;
typedef struct _DaxAnimateElementClass DaxAnimateElementClass;
typedef struct _DaxAnimateElementPrivate DaxAnimateElementPrivate;

struct _DaxAnimateElement
{
    DaxElement parent;

    DaxAnimateElementPrivate *priv;
};

struct _DaxAnimateElementClass
{
    DaxElementClass parent_class;
};

GType                       dax_animate_element_get_type             (void) G_GNUC_CONST;

DaxDomElement *          dax_animate_element_new                  (void);
DaxDuration *            dax_animate_element_get_duration         (const DaxAnimateElement *self);
const gchar *               dax_animate_element_get_attribute_name   (const DaxAnimateElement *self);
const gchar *               dax_animate_element_get_from             (const DaxAnimateElement *self);
const gchar *               dax_animate_element_get_to               (const DaxAnimateElement *self);
const DaxRepeatCount *   dax_animate_element_get_repeat_count     (const DaxAnimateElement *self);
DaxDomElement *          dax_animate_element_get_target           (const DaxAnimateElement *self);

G_END_DECLS

#endif /* __DAX_ANIMATE_ELEMENT_H__ */
