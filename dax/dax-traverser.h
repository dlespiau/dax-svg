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

#ifndef __DAX_TRAVERSER_H__
#define __DAX_TRAVERSER_H__

#include <glib-object.h>

#include "dax-dom-node.h"

#include "dax-animate-element.h"
#include "dax-circle-element.h"
#include "dax-g-element.h"
#include "dax-handler-element.h"
#include "dax-path-element.h"
#include "dax-polyline-element.h"
#include "dax-rect-element.h"
#include "dax-script-element.h"
#include "dax-svg-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_TRAVERSER dax_traverser_get_type()

#define DAX_TRAVERSER(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 DAX_TYPE_TRAVERSER, \
                                 DaxTraverser))

#define DAX_TRAVERSER_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_TRAVERSER,    \
                              DaxTraverserClass))

#define DAX_IS_TRAVERSER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_TRAVERSER))

#define DAX_IS_TRAVERSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_TRAVERSER))

#define DAX_TRAVERSER_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_TRAVERSER,  \
                                DaxTraverserClass))

typedef struct _DaxTraverser DaxTraverser;
typedef struct _DaxTraverserClass DaxTraverserClass;
typedef struct _DaxTraverserPrivate DaxTraverserPrivate;

struct _DaxTraverser
{
    GObject parent;

    DaxTraverserPrivate *priv;
};

struct _DaxTraverserClass
{
    GObjectClass parent_class;

    void (*traverse_svg)        (DaxTraverser  *traverser,
                                 DaxSvgElement *node);
    void (*traverse_g)          (DaxTraverser *traverser,
                                 DaxGElement  *node);
    void (*traverse_path)       (DaxTraverser   *traverser,
                                 DaxPathElement *node);
    void (*traverse_rect)       (DaxTraverser   *traverser,
                                 DaxRectElement *node);
    void (*traverse_polyline)   (DaxTraverser       *traverser,
                                 DaxPolylineElement *node);
    void (*traverse_animate)    (DaxTraverser       *traverser,
                                 DaxAnimateElement  *node);
    void (*traverse_circle)     (DaxTraverser      *traverser,
                                 DaxCircleElement  *node);
    void (*traverse_script)     (DaxTraverser      *traverser,
                                 DaxScriptElement  *node);
    void (*traverse_handler)    (DaxTraverser       *traverser,
                                 DaxHandlerElement  *node);
};

GType            dax_traverser_get_type       (void) G_GNUC_CONST;

DaxTraverser *dax_traverser_new               (DaxDomNode *root);
void             dax_traverser_set_root          (DaxTraverser *self,
                                                     DaxDomNode   *root);
void             dax_traverser_apply             (DaxTraverser *self);

void             dax_traverser_traverse_svg      (DaxTraverser  *self,
                                                     DaxSvgElement *node);
void             dax_traverser_traverse_g        (DaxTraverser *self,
                                                     DaxGElement  *node);
void             dax_traverser_traverse_path     (DaxTraverser   *self,
                                                     DaxPathElement *node);
void             dax_traverser_traverse_rect     (DaxTraverser   *self,
                                                     DaxRectElement *node);
void             dax_traverser_traverse_polyline (DaxTraverser      *self,
                                                     DaxPolylineElement *node);
void             dax_traverser_traverse_animate  (DaxTraverser      *self,
                                                     DaxAnimateElement *node);
void             dax_traverser_traverse_circle   (DaxTraverser     *self,
                                                     DaxCircleElement *node);
void             dax_traverser_traverse_script   (DaxTraverser     *self,
                                                     DaxScriptElement *node);
void             dax_traverser_traverse_handler  (DaxTraverser      *self,
                                                     DaxHandlerElement *node);

G_END_DECLS

#endif /* __DAX_TRAVERSER_H__ */
