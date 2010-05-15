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

#ifndef __DAX_TRAVERSER_CLUTTER_H__
#define __DAX_TRAVERSER_CLUTTER_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-traverser.h"

G_BEGIN_DECLS

#define DAX_TYPE_TRAVERSER_CLUTTER dax_traverser_clutter_get_type()

#define DAX_TRAVERSER_CLUTTER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_TRAVERSER_CLUTTER,    \
                                 DaxTraverserClutter))

#define DAX_TRAVERSER_CLUTTER_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_TRAVERSER_CLUTTER,   \
                              DaxTraverserClutterClass))

#define DAX_IS_TRAVERSER_CLUTTER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_TRAVERSER_CLUTTER))

#define DAX_IS_TRAVERSER_CLUTTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_TRAVERSER_CLUTTER))

#define DAX_TRAVERSER_CLUTTER_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_TRAVERSER_CLUTTER, \
                                DaxTraverserClutterClass))

typedef struct _DaxTraverserClutter DaxTraverserClutter;
typedef struct _DaxTraverserClutterClass DaxTraverserClutterClass;
typedef struct _DaxTraverserClutterPrivate DaxTraverserClutterPrivate;

struct _DaxTraverserClutter
{
    DaxTraverser parent;

    DaxTraverserClutterPrivate *priv;
};

struct _DaxTraverserClutterClass
{
    DaxTraverserClass parent_class;
};

GType           dax_traverser_clutter_get_type          (void) G_GNUC_CONST;

DaxTraverser *  dax_traverser_clutter_new               (DaxDomNode    *node,
                                                         ClutterContainer *container);
void            dax_traverser_clutter_set_container     (DaxTraverserClutter *self,
                                                         ClutterContainer     *container);
ClutterScore *  dax_traverser_clutter_get_score         (DaxTraverserClutter *self);

G_END_DECLS

#endif /* __DAX_TRAVERSER_CLUTTER_H__ */
