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

#include "dax-debug.h"
#include "dax-traverser.h"

G_DEFINE_TYPE (DaxTraverser, dax_traverser, G_TYPE_OBJECT)

#define TRAVERSER_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_TRAVERSER,    \
                                      DaxTraverserPrivate))

struct _DaxTraverserPrivate
{
    DaxDomNode *root;
};

static void
dax_traverser_traverse_svg_real (DaxTraverser  *self,
                                    DaxSvgElement *node)
{
}

static void
dax_traverser_traverse_g_real (DaxTraverser *self,
                                  DaxGElement  *node)
{
}

static void
dax_traverser_traverse_path_real (DaxTraverser   *self,
                                     DaxPathElement *node)
{
}

static void
dax_traverser_traverse_rect_real (DaxTraverser   *self,
                                     DaxRectElement *node)
{
}

static void
dax_traverser_traverse_polyline_real (DaxTraverser      *self,
                                         DaxPolylineElement *node)
{
}

static void
dax_traverser_traverse_animate_real (DaxTraverser      *self,
                                        DaxAnimateElement *node)
{
}

static void
dax_traverser_traverse_circle_real (DaxTraverser     *self,
                                       DaxCircleElement *node)
{
}

static void
dax_traverser_traverse_script_real (DaxTraverser     *self,
                                       DaxScriptElement *node)
{
}

static void
dax_traverser_traverse_handler_real (DaxTraverser      *self,
                                        DaxHandlerElement *node)
{
}

static void
dax_traverser_traverse_line_real (DaxTraverser *self,
                                  DaxLine      *node)
{
}

/*
 * GObject overloading
 */

static void
dax_traverser_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_traverser_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_traverser_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_traverser_parent_class)->dispose (object);
}

static void
dax_traverser_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_traverser_parent_class)->finalize (object);
}

static void
dax_traverser_class_init (DaxTraverserClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxTraverserPrivate));

    object_class->get_property = dax_traverser_get_property;
    object_class->set_property = dax_traverser_set_property;
    object_class->dispose = dax_traverser_dispose;
    object_class->finalize = dax_traverser_finalize;

    klass->traverse_svg = dax_traverser_traverse_svg_real;
    klass->traverse_g = dax_traverser_traverse_g_real;
    klass->traverse_path = dax_traverser_traverse_path_real;
    klass->traverse_rect = dax_traverser_traverse_rect_real;
    klass->traverse_polyline = dax_traverser_traverse_polyline_real;
    klass->traverse_animate = dax_traverser_traverse_animate_real;
    klass->traverse_circle = dax_traverser_traverse_circle_real;
    klass->traverse_script = dax_traverser_traverse_script_real;
    klass->traverse_handler = dax_traverser_traverse_handler_real;
    klass->traverse_line = dax_traverser_traverse_line_real;
}

static void
dax_traverser_init (DaxTraverser *self)
{
    self->priv = TRAVERSER_PRIVATE (self);
}

DaxTraverser *
dax_traverser_new (DaxDomNode *root)
{
    DaxTraverser *traverser;

    traverser = g_object_new (DAX_TYPE_TRAVERSER, NULL);
    dax_traverser_set_root (traverser, root);

    return traverser;
}

void
dax_traverser_set_root (DaxTraverser *self,
                           DaxDomNode   *root)
{
    DaxTraverserPrivate *priv;

    g_return_if_fail (DAX_IS_TRAVERSER (self));

    priv = self->priv;
    if (priv->root)
        g_object_unref (priv->root);
    priv->root = g_object_ref (root);
}

static void
dax_traverse_node (DaxTraverser *traverser,
                      DaxDomNode   *node)
{
    DAX_NOTE (TRAVERSER, "traversing %s %p", G_OBJECT_TYPE_NAME (node),
                 node);

    if (DAX_IS_SVG_ELEMENT (node))
        dax_traverser_traverse_svg (traverser, (DaxSvgElement *)node);
    else if (DAX_IS_G_ELEMENT (node))
        dax_traverser_traverse_g (traverser, (DaxGElement *)node);
    else if (DAX_IS_PATH_ELEMENT (node))
        dax_traverser_traverse_path (traverser, (DaxPathElement *)node);
    else if (DAX_IS_RECT_ELEMENT (node))
        dax_traverser_traverse_rect (traverser, (DaxRectElement *)node);
    else if (DAX_IS_POLYLINE_ELEMENT (node))
        dax_traverser_traverse_polyline (traverser, (DaxPolylineElement *)node);
    else if (DAX_IS_ANIMATE_ELEMENT (node))
        dax_traverser_traverse_animate (traverser, (DaxAnimateElement *)node);
    else if (DAX_IS_CIRCLE_ELEMENT (node))
        dax_traverser_traverse_circle (traverser, (DaxCircleElement *)node);
    else if (DAX_IS_SCRIPT_ELEMENT (node))
        dax_traverser_traverse_script (traverser, (DaxScriptElement *)node);
    else if (DAX_IS_HANDLER_ELEMENT (node))
        dax_traverser_traverse_handler (traverser, (DaxHandlerElement *)node);
    else if (DAX_IS_LINE (node))
        dax_traverser_traverse_line (traverser, (DaxLine *)node);
}

static void
dax_traverser_walk_tree (DaxTraverser *traverser,
                            DaxDomNode   *node)
{
    dax_traverse_node (traverser, node);
    node = node->first_child;
    while (node) {
        dax_traverser_walk_tree (traverser, node);
        node = node->next_sibling;
    }
}

void
dax_traverser_apply (DaxTraverser *self)
{
    DaxTraverserPrivate *priv;

    g_return_if_fail (DAX_IS_TRAVERSER (self));

    priv = self->priv;
    if (priv->root)
        dax_traverser_walk_tree (self, priv->root);
}

void
dax_traverser_traverse_svg (DaxTraverser  *self,
                               DaxSvgElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_svg (self, node);
}

void
dax_traverser_traverse_g (DaxTraverser *self,
                             DaxGElement  *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_g (self, node);
}

void
dax_traverser_traverse_path (DaxTraverser   *self,
                                DaxPathElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_path (self, node);
}

void
dax_traverser_traverse_rect (DaxTraverser   *self,
                                DaxRectElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_rect (self, node);
}

void
dax_traverser_traverse_polyline (DaxTraverser      *self,
                                    DaxPolylineElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_polyline (self, node);
}

void
dax_traverser_traverse_animate (DaxTraverser      *self,
                                   DaxAnimateElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_animate (self, node);
}

void
dax_traverser_traverse_circle (DaxTraverser     *self,
                                  DaxCircleElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_circle (self, node);
}

void
dax_traverser_traverse_script (DaxTraverser     *self,
                                  DaxScriptElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_script (self, node);
}

void
dax_traverser_traverse_handler (DaxTraverser      *self,
                                   DaxHandlerElement *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_handler (self, node);
}

void
dax_traverser_traverse_line (DaxTraverser *self,
                             DaxLine      *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_line (self, node);
}
