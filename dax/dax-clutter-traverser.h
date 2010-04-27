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

#ifndef __DAX_CLUTTER_TRAVERSER_H__
#define __DAX_CLUTTER_TRAVERSER_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-traverser.h"
#include "dax-clutter-traverser.h"

G_BEGIN_DECLS

#define DAX_TYPE_CLUTTER_TRAVERSER dax_clutter_traverser_get_type()

#define DAX_CLUTTER_TRAVERSER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_CLUTTER_TRAVERSER,    \
                                 DaxClutterTraverser))

#define DAX_CLUTTER_TRAVERSER_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_CLUTTER_TRAVERSER,   \
                              DaxClutterTraverserClass))

#define DAX_IS_CLUTTER_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_CLUTTER_TRAVERSER))

#define DAX_IS_CLUTTER_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_CLUTTER_TRAVERSER))

#define DAX_CLUTTER_TRAVERSER_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_CLUTTER_TRAVERSER, \
                                DaxClutterTraverserClass))

typedef struct _DaxClutterTraverser DaxClutterTraverser;
typedef struct _DaxClutterTraverserClass DaxClutterTraverserClass;
typedef struct _DaxClutterTraverserPrivate DaxClutterTraverserPrivate;

struct _DaxClutterTraverser
{
    DaxTraverser parent;

    DaxClutterTraverserPrivate *priv;
};

struct _DaxClutterTraverserClass
{
    DaxTraverserClass parent_class;
};

GType           dax_clutter_traverser_get_type          (void) G_GNUC_CONST;

DaxTraverser *  dax_clutter_traverser_new               (DaxDomNode    *node,
                                                         ClutterContainer *container);
void            dax_clutter_traverser_set_container     (DaxClutterTraverser *self,
                                                         ClutterContainer     *container);
ClutterScore *  dax_clutter_traverser_get_score         (DaxClutterTraverser *self);

G_END_DECLS

#endif /* __DAX_CLUTTER_TRAVERSER_H__ */
