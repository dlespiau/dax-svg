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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_BUILD_TRAVERSER_H__
#define __DAX_BUILD_TRAVERSER_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-traverser.h"
#include "dax-build-traverser.h"

G_BEGIN_DECLS

#define DAX_TYPE_BUILD_TRAVERSER dax_build_traverser_get_type()

#define DAX_BUILD_TRAVERSER(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_BUILD_TRAVERSER,   \
                                 DaxBuildTraverser))

#define DAX_BUILD_TRAVERSER_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              DAX_TYPE_BUILD_TRAVERSER,  \
                              DaxBuildTraverserClass))

#define DAX_IS_BUILD_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_BUILD_TRAVERSER))

#define DAX_IS_BUILD_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_BUILD_TRAVERSER))

#define DAX_BUILD_TRAVERSER_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_BUILD_TRAVERSER,    \
                                DaxBuildTraverserClass))

typedef struct _DaxBuildTraverser DaxBuildTraverser;
typedef struct _DaxBuildTraverserClass DaxBuildTraverserClass;
typedef struct _DaxBuildTraverserPrivate DaxBuildTraverserPrivate;

struct _DaxBuildTraverser
{
    DaxTraverser parent;

    DaxBuildTraverserPrivate *priv;
};

struct _DaxBuildTraverserClass
{
    DaxTraverserClass parent_class;
};

GType                   dax_build_traverser_get_type         (void) G_GNUC_CONST;

DaxTraverser *       dax_build_traverser_new              (DaxDomNode    *node,
                                                                 ClutterContainer *container);
void                    dax_build_traverser_set_container    (DaxBuildTraverser *self,
                                                                 ClutterContainer     *container);
ClutterScore *          dax_build_traverser_get_score        (DaxBuildTraverser *self);

G_END_DECLS

#endif /* __DAX_BUILD_TRAVERSER_H__ */
