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

#ifndef __CASTET_TRAVERSER_H__
#define __CASTET_TRAVERSER_H__

#include <glib-object.h>

#include "castet-dom-node.h"

#include "castet-animate-element.h"
#include "castet-circle-element.h"
#include "castet-g-element.h"
#include "castet-handler-element.h"
#include "castet-path-element.h"
#include "castet-polyline-element.h"
#include "castet-rect-element.h"
#include "castet-script-element.h"
#include "castet-svg-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_TRAVERSER castet_traverser_get_type()

#define CASTET_TRAVERSER(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 CASTET_TYPE_TRAVERSER, \
                                 CastetTraverser))

#define CASTET_TRAVERSER_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CASTET_TYPE_TRAVERSER,    \
                              CastetTraverserClass))

#define CASTET_IS_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_TRAVERSER))

#define CASTET_IS_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_TRAVERSER))

#define CASTET_TRAVERSER_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_TRAVERSER,  \
                                CastetTraverserClass))

typedef struct _CastetTraverser CastetTraverser;
typedef struct _CastetTraverserClass CastetTraverserClass;
typedef struct _CastetTraverserPrivate CastetTraverserPrivate;

struct _CastetTraverser
{
    GObject parent;

    CastetTraverserPrivate *priv;
};

struct _CastetTraverserClass
{
    GObjectClass parent_class;

    void (*traverse_svg)        (CastetTraverser  *traverser,
                                 CastetSvgElement *node);
    void (*traverse_g)          (CastetTraverser *traverser,
                                 CastetGElement  *node);
    void (*traverse_path)       (CastetTraverser   *traverser,
                                 CastetPathElement *node);
    void (*traverse_rect)       (CastetTraverser   *traverser,
                                 CastetRectElement *node);
    void (*traverse_polyline)   (CastetTraverser       *traverser,
                                 CastetPolylineElement *node);
    void (*traverse_animate)    (CastetTraverser       *traverser,
                                 CastetAnimateElement  *node);
    void (*traverse_circle)     (CastetTraverser      *traverser,
                                 CastetCircleElement  *node);
    void (*traverse_script)     (CastetTraverser      *traverser,
                                 CastetScriptElement  *node);
    void (*traverse_handler)    (CastetTraverser       *traverser,
                                 CastetHandlerElement  *node);
};

GType            castet_traverser_get_type       (void) G_GNUC_CONST;

CastetTraverser *castet_traverser_new               (CastetDomNode *root);
void             castet_traverser_set_root          (CastetTraverser *self,
                                                     CastetDomNode   *root);
void             castet_traverser_apply             (CastetTraverser *self);

void             castet_traverser_traverse_svg      (CastetTraverser  *self,
                                                     CastetSvgElement *node);
void             castet_traverser_traverse_g        (CastetTraverser *self,
                                                     CastetGElement  *node);
void             castet_traverser_traverse_path     (CastetTraverser   *self,
                                                     CastetPathElement *node);
void             castet_traverser_traverse_rect     (CastetTraverser   *self,
                                                     CastetRectElement *node);
void             castet_traverser_traverse_polyline (CastetTraverser      *self,
                                                     CastetPolylineElement *node);
void             castet_traverser_traverse_animate  (CastetTraverser      *self,
                                                     CastetAnimateElement *node);
void             castet_traverser_traverse_circle   (CastetTraverser     *self,
                                                     CastetCircleElement *node);
void             castet_traverser_traverse_script   (CastetTraverser     *self,
                                                     CastetScriptElement *node);
void             castet_traverser_traverse_handler  (CastetTraverser      *self,
                                                     CastetHandlerElement *node);

G_END_DECLS

#endif /* __CASTET_TRAVERSER_H__ */
