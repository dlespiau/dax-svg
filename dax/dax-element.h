/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#ifndef __DAX_ELEMENT_H__
#define __DAX_ELEMENT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-dom-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT dax_element_get_type()

#define DAX_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ELEMENT, DaxElement))

#define DAX_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_ELEMENT, DaxElementClass))

#define DAX_IS_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT))

#define DAX_IS_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT))

#define DAX_ELEMENT_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_ELEMENT, DaxElementClass))

typedef struct _DaxElement DaxElement;
typedef struct _DaxElementClass DaxElementClass;
typedef struct _DaxElementPrivate DaxElementPrivate;

struct _DaxElement
{
    DaxDomElement parent;

    DaxElementPrivate *priv;
};

struct _DaxElementClass
{
    DaxDomElementClass parent_class;
};

GType dax_element_get_type (void) G_GNUC_CONST;

DaxDomElement *         dax_element_new                 (void);
const ClutterColor *    dax_element_get_fill_color      (DaxElement *element);
const ClutterColor *    dax_element_get_stroke_color    (DaxElement *element);
gfloat                  dax_element_get_fill_opacity    (DaxElement *element);

const gchar *           dax_element_get_base_iri        (DaxElement *element);

/*
 * Trait API
 */

#define dax_element_get_float_trait(element, name) \
    dax_element_getFloatTrait(element,name)
#define dax_element_set_float_trait(element, name, value) \
    dax_element_setFloatTrait(element, name, value)

gfloat                  dax_element_getFloatTrait       (DaxElement *element,
                                                         const char    *name);
void                    dax_element_setFloatTrait       (DaxElement *element,
                                                         const char    *name,
                                                         gfloat         value);

G_END_DECLS

#endif /* __DAX_ELEMENT_H__ */
