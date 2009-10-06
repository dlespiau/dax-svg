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

#include "castet-traverser.h"

G_DEFINE_TYPE (CastetTraverser, castet_traverser, G_TYPE_OBJECT)

#define TRAVERSER_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_TRAVERSER,    \
                                      CastetTraverserPrivate))

struct _CastetTraverserPrivate
{
    CastetDomNode *root;
};

static void
castet_traverser_traverse_svg_real (CastetTraverser  *self,
                                    CastetSvgElement *node)
{
}

static void
castet_traverser_traverse_g_real (CastetTraverser *self,
                                  CastetGElement  *node)
{
}

static void
castet_traverser_traverse_path_real (CastetTraverser   *self,
                                     CastetPathElement *node)
{
}

static void
castet_traverser_traverse_rect_real (CastetTraverser   *self,
                                     CastetRectElement *node)
{
}

static void
castet_traverser_traverse_polyline_real (CastetTraverser      *self,
                                         CastetPolylineElement *node)
{
}

/*
 * GObject overloading
 */

static void
castet_traverser_get_property (GObject    *object,
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
castet_traverser_set_property (GObject      *object,
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
castet_traverser_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_traverser_parent_class)->dispose (object);
}

static void
castet_traverser_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_traverser_parent_class)->finalize (object);
}

static void
castet_traverser_class_init (CastetTraverserClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetTraverserPrivate));

    object_class->get_property = castet_traverser_get_property;
    object_class->set_property = castet_traverser_set_property;
    object_class->dispose = castet_traverser_dispose;
    object_class->finalize = castet_traverser_finalize;

    klass->traverse_svg = castet_traverser_traverse_svg_real;
    klass->traverse_g = castet_traverser_traverse_g_real;
    klass->traverse_path = castet_traverser_traverse_path_real;
    klass->traverse_rect = castet_traverser_traverse_rect_real;
    klass->traverse_polyline = castet_traverser_traverse_polyline_real;
}

static void
castet_traverser_init (CastetTraverser *self)
{
    self->priv = TRAVERSER_PRIVATE (self);
}

CastetTraverser *
castet_traverser_new (CastetDomNode *root)
{
    CastetTraverser *traverser;

    traverser = g_object_new (CASTET_TYPE_TRAVERSER, NULL);
    castet_traverser_set_root (traverser, root);

    return traverser;
}

void
castet_traverser_set_root (CastetTraverser *self,
                           CastetDomNode   *root)
{
    CastetTraverserPrivate *priv;

    g_return_if_fail (CASTET_IS_TRAVERSER (self));

    priv = self->priv;
    if (priv->root)
        g_object_unref (priv->root);
    priv->root = g_object_ref (root);
}

static void
castet_traverse_node (CastetTraverser *traverser,
                      CastetDomNode   *node)
{
    if (CASTET_IS_SVG_ELEMENT (node))
        castet_traverser_traverse_svg (traverser, (CastetSvgElement *)node);
    else if (CASTET_IS_G_ELEMENT (node))
        castet_traverser_traverse_g (traverser, (CastetGElement *)node);
    else if (CASTET_IS_PATH_ELEMENT (node))
        castet_traverser_traverse_path (traverser, (CastetPathElement *)node);
    else if (CASTET_IS_RECT_ELEMENT (node))
        castet_traverser_traverse_rect (traverser, (CastetRectElement *)node);
    else if (CASTET_IS_POLYLINE_ELEMENT (node))
        castet_traverser_traverse_polyline (traverser,
                                            (CastetPolylineElement *)node);
}

static void
castet_traverser_walk_tree (CastetTraverser *traverser,
                            CastetDomNode   *node)
{
    castet_traverse_node (traverser, node);
    if (node->first_child)
        castet_traverser_walk_tree (traverser, node->first_child);
    while (node->next_sibling) {
        castet_traverser_walk_tree (traverser, node->next_sibling);
        node = node->next_sibling;
    }
}

void
castet_traverser_apply (CastetTraverser *self)
{
    CastetTraverserPrivate *priv;

    g_return_if_fail (CASTET_IS_TRAVERSER (self));

    priv = self->priv;
    if (priv->root)
        castet_traverser_walk_tree (self, priv->root);
}

void
castet_traverser_traverse_svg (CastetTraverser  *self,
                               CastetSvgElement *node)
{
    CastetTraverserClass *klass = CASTET_TRAVERSER_GET_CLASS (self);

    klass->traverse_svg (self, node);
}

void
castet_traverser_traverse_g (CastetTraverser *self,
                             CastetGElement  *node)
{
    CastetTraverserClass *klass = CASTET_TRAVERSER_GET_CLASS (self);

    klass->traverse_g (self, node);
}

void
castet_traverser_traverse_path (CastetTraverser   *self,
                                CastetPathElement *node)
{
    CastetTraverserClass *klass = CASTET_TRAVERSER_GET_CLASS (self);

    klass->traverse_path (self, node);
}

void
castet_traverser_traverse_rect (CastetTraverser   *self,
                                CastetRectElement *node)
{
    CastetTraverserClass *klass = CASTET_TRAVERSER_GET_CLASS (self);

    klass->traverse_rect (self, node);
}

void
castet_traverser_traverse_polyline (CastetTraverser      *self,
                                    CastetPolylineElement *node)
{
    CastetTraverserClass *klass = CASTET_TRAVERSER_GET_CLASS (self);

    klass->traverse_polyline (self, node);
}
