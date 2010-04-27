/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#ifndef __DAX_ELEMENT_TSPAN_H__
#define __DAX_ELEMENT_TSPAN_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_TSPAN dax_element_tspan_get_type()

#define DAX_ELEMENT_TSPAN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ELEMENT_TSPAN, DaxElementTspan))

#define DAX_ELEMENT_TSPAN_CLASS(klass)              \
  (G_TYPE_CHECK_CLASS_CAST ((klass),                \
                            DAX_TYPE_ELEMENT_TSPAN, \
                            DaxElementTspanClass))

#define DAX_IS_ELEMENT_TSPAN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_TSPAN))

#define DAX_IS_ELEMENT_TSPAN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_TSPAN))

#define DAX_ELEMENT_TSPAN_GET_CLASS(obj)                \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                    \
                              DAX_TYPE_ELEMENT_TSPAN,   \
                              DaxElementTspanClass))

typedef struct _DaxElementTspan DaxElementTspan;
typedef struct _DaxElementTspanClass DaxElementTspanClass;
typedef struct _DaxElementTspanPrivate DaxElementTspanPrivate;

struct _DaxElementTspan
{
  DaxElement parent;

  DaxElementTspanPrivate *priv;
};

struct _DaxElementTspanClass
{
  DaxElementClass parent_class;
};

GType dax_element_tspan_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_element_tspan_new (void);

G_END_DECLS

#endif /* __DAX_ELEMENT_TSPAN_H__ */
