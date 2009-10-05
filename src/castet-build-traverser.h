/*
 * Castet - Load and draw SVG
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

#ifndef __CASTET_BUILD_TRAVERSER_H__
#define __CASTET_BUILD_TRAVERSER_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "castet-traverser.h"
#include "castet-build-traverser.h"

G_BEGIN_DECLS

#define CASTET_TYPE_BUILD_TRAVERSER castet_build_traverser_get_type()

#define CASTET_BUILD_TRAVERSER(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 CASTET_TYPE_BUILD_TRAVERSER,   \
                                 CastetBuildTraverser))

#define CASTET_BUILD_TRAVERSER_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              CASTET_TYPE_BUILD_TRAVERSER,  \
                              CastetBuildTraverserClass))

#define CASTET_IS_BUILD_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_BUILD_TRAVERSER))

#define CASTET_IS_BUILD_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_BUILD_TRAVERSER))

#define CASTET_BUILD_TRAVERSER_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                CASTET_TYPE_BUILD_TRAVERSER,    \
                                CastetBuildTraverserClass))

typedef struct _CastetBuildTraverser CastetBuildTraverser;
typedef struct _CastetBuildTraverserClass CastetBuildTraverserClass;
typedef struct _CastetBuildTraverserPrivate CastetBuildTraverserPrivate;

struct _CastetBuildTraverser
{
    CastetTraverser parent;

    CastetBuildTraverserPrivate *priv;
};

struct _CastetBuildTraverserClass
{
    CastetTraverserClass parent_class;
};

GType                   castet_build_traverser_get_type         (void) G_GNUC_CONST;

CastetTraverser        *castet_build_traverser_new              (CastetDomNode    *node,
                                                                 ClutterContainer *container);
void                    castet_build_traverser_set_container    (CastetBuildTraverser *self,
                                                                 ClutterContainer     *container);

G_END_DECLS

#endif /* __CASTET_BUILD_TRAVERSER_H__ */
