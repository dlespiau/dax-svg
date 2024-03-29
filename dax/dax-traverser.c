/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include "dax-affine.h"
#include "dax-debug.h"
#include "dax-internals.h"

#include "dax-traverser.h"

G_DEFINE_ABSTRACT_TYPE (DaxTraverser, dax_traverser, G_TYPE_OBJECT)

#define TRAVERSER_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_TRAVERSER,   \
                                      DaxTraverserPrivate))

enum
{
    PROP_0,

    PROP_ROOT
};

struct _DaxTraverserPrivate
{
    DaxMatrix ctm;
    DaxDomNode *root;
};

/* Update the ctm with the transform matrix set on element */
static void
apply_transform (DaxTraverser *traverser,
                 DaxElement   *element)
{
    DaxTraverserPrivate *priv = traverser->priv;
    DaxMatrix *matrix;

    g_object_get (element, "transform", &matrix, NULL);
    if (matrix == NULL)
        return;

    _dax_affine_multiply (priv->ctm.affine, priv->ctm.affine, matrix->affine);
    dax_matrix_free (matrix);
}

/*
 * Provide some default vfunc implementations to not have a test in the
 * dax_traverser_traverse_*() wrappers.
 */

static void
dax_traverser_traverse_svg_real (DaxTraverser  *self,
                                 DaxElementSvg *node)
{
}

static void
dax_traverser_traverse_g_real (DaxTraverser    *self,
                               DaxElementG     *node,
                               DaxTraverserWay  way)
{
}

static void
dax_traverser_traverse_path_real (DaxTraverser   *self,
                                  DaxElementPath *node)
{
}

static void
dax_traverser_traverse_rect_real (DaxTraverser   *self,
                                  DaxElementRect *node)
{
}

static void
dax_traverser_traverse_polyline_real (DaxTraverser       *self,
                                      DaxElementPolyline *node)
{
}

static void
dax_traverser_traverse_animate_real (DaxTraverser      *self,
                                     DaxElementAnimate *node)
{
}

static void
dax_traverser_traverse_animate_transform_real (DaxTraverser               *self,
                                               DaxElementAnimateTransform *node)
{
}

static void
dax_traverser_traverse_circle_real (DaxTraverser     *self,
                                    DaxElementCircle *node)
{
}

static void
dax_traverser_traverse_script_real (DaxTraverser     *self,
                                    DaxElementScript *node)
{
}

static void
dax_traverser_traverse_handler_real (DaxTraverser      *self,
                                     DaxElementHandler *node)
{
}

static void
dax_traverser_traverse_line_real (DaxTraverser   *self,
                                  DaxElementLine *node)
{
}

static void
dax_traverser_traverse_text_real (DaxTraverser   *self,
                                  DaxElementText *node)
{
}

static void
dax_traverser_traverse_image_real (DaxTraverser    *self,
                                   DaxElementImage *node)
{
}

static void
dax_traverser_traverse_video_real (DaxTraverser    *self,
                                   DaxElementVideo *node)
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
    DaxTraverser *traverser = DAX_TRAVERSER (object);
    DaxTraverserPrivate *priv = traverser->priv;

    switch (property_id)
    {
    case PROP_ROOT:
        g_value_set_object (value, priv->root);
        break;
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
    DaxTraverser *traverser = DAX_TRAVERSER (object);
    DaxTraverserPrivate *priv = traverser->priv;

    switch (property_id)
    {
    case PROP_ROOT:
        if (priv->root)
            g_object_unref (priv->root);
        priv->root = g_value_dup_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_traverser_dispose (GObject *object)
{
    DaxTraverser *traverser = DAX_TRAVERSER (object);
    DaxTraverserPrivate *priv = traverser->priv;

    if (priv->root) {
        g_object_unref (priv->root);
        priv->root = NULL;
    }

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
    GParamSpec *pspec;

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
    klass->traverse_animate_transform =
        dax_traverser_traverse_animate_transform_real;
    klass->traverse_circle = dax_traverser_traverse_circle_real;
    klass->traverse_script = dax_traverser_traverse_script_real;
    klass->traverse_handler = dax_traverser_traverse_handler_real;
    klass->traverse_line = dax_traverser_traverse_line_real;
    klass->traverse_text = dax_traverser_traverse_text_real;
    klass->traverse_image = dax_traverser_traverse_image_real;
    klass->traverse_video = dax_traverser_traverse_video_real;

    pspec = g_param_spec_object ("root",
                                 "Root",
                                 "Root element from which the traverser "
                                 "will be applied",
                                 DAX_TYPE_DOM_NODE,
                                 DAX_GPARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (object_class, PROP_ROOT, pspec);
}

static void
dax_traverser_init (DaxTraverser *self)
{
    DaxTraverserPrivate *priv;

    self->priv = priv = TRAVERSER_PRIVATE (self);

    _dax_affine_identity (priv->ctm.affine);
}

const DaxMatrix *
dax_traverser_get_ctm (DaxTraverser *self)
{
    g_return_val_if_fail (DAX_IS_TRAVERSER (self), NULL);

    return &self->priv->ctm;
}

static void
dax_traverse_node (DaxTraverser    *traverser,
                   DaxDomNode      *node,
                   DaxTraverserWay  way)
{
    DAX_NOTE (TRAVERSER, "traversing %s %p", G_OBJECT_TYPE_NAME (node), node);

    if (way == DAX_TRAVERSER_WAY_END) {
        if (DAX_IS_ELEMENT_G (node)) {
            dax_traverser_traverse_g (traverser, (DaxElementG *)node, way);
        }
        return;
    }

    if (DAX_IS_ELEMENT_SVG (node))
        dax_traverser_traverse_svg (traverser, (DaxElementSvg *)node);
    else if (DAX_IS_ELEMENT_G (node))
        dax_traverser_traverse_g (traverser, (DaxElementG *)node, way);
    else if (DAX_IS_ELEMENT_PATH (node))
        dax_traverser_traverse_path (traverser, (DaxElementPath *)node);
    else if (DAX_IS_ELEMENT_RECT (node))
        dax_traverser_traverse_rect (traverser, (DaxElementRect *)node);
    else if (DAX_IS_ELEMENT_TEXT (node))
        dax_traverser_traverse_text (traverser, (DaxElementText *)node);
    else if (DAX_IS_ELEMENT_POLYLINE (node))
        dax_traverser_traverse_polyline (traverser, (DaxElementPolyline *)node);
    else if (DAX_IS_ELEMENT_ANIMATE (node))
        dax_traverser_traverse_animate (traverser, (DaxElementAnimate *)node);
    else if (DAX_IS_ELEMENT_ANIMATE_TRANSFORM (node)) {
        DaxElementAnimateTransform *element;

        element = DAX_ELEMENT_ANIMATE_TRANSFORM (node);
        dax_traverser_traverse_animate_transform (traverser, element);
    } else if (DAX_IS_ELEMENT_IMAGE (node))
        dax_traverser_traverse_image (traverser, (DaxElementImage *)node);
    else if (DAX_IS_ELEMENT_VIDEO (node))
        dax_traverser_traverse_video (traverser, (DaxElementVideo *)node);
    else if (DAX_IS_ELEMENT_CIRCLE (node))
        dax_traverser_traverse_circle (traverser, (DaxElementCircle *)node);
    else if (DAX_IS_ELEMENT_SCRIPT (node))
        dax_traverser_traverse_script (traverser, (DaxElementScript *)node);
    else if (DAX_IS_ELEMENT_HANDLER (node))
        dax_traverser_traverse_handler (traverser, (DaxElementHandler *)node);
    else if (DAX_IS_ELEMENT_LINE (node))
        dax_traverser_traverse_line (traverser, (DaxElementLine *)node);
}

static void
dax_traverser_walk_tree (DaxTraverser *traverser,
                         DaxDomNode   *node)
{
    DaxDomNode *cur;

    dax_traverse_node (traverser, node, DAX_TRAVERSER_WAY_START);
    cur = node->first_child;
    while (cur) {
        dax_traverser_walk_tree (traverser, cur);
        cur = cur->next_sibling;
    }
    dax_traverse_node (traverser, node, DAX_TRAVERSER_WAY_END);
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
                            DaxElementSvg *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_svg (self, node);
}

void
dax_traverser_traverse_g (DaxTraverser    *self,
                          DaxElementG     *node,
                          DaxTraverserWay  way)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    apply_transform (self, DAX_ELEMENT (node));
    klass->traverse_g (self, node, way);
}

void
dax_traverser_traverse_path (DaxTraverser   *self,
                             DaxElementPath *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    apply_transform (self, DAX_ELEMENT (node));
    klass->traverse_path (self, node);
}

void
dax_traverser_traverse_rect (DaxTraverser   *self,
                             DaxElementRect *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_rect (self, node);
}

void
dax_traverser_traverse_polyline (DaxTraverser       *self,
                                 DaxElementPolyline *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_polyline (self, node);
}

void
dax_traverser_traverse_animate (DaxTraverser      *self,
                                DaxElementAnimate *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_animate (self, node);
}

void
dax_traverser_traverse_animate_transform (DaxTraverser               *self,
                                          DaxElementAnimateTransform *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_animate_transform (self, node);
}

void
dax_traverser_traverse_circle (DaxTraverser     *self,
                               DaxElementCircle *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_circle (self, node);
}

void
dax_traverser_traverse_script (DaxTraverser     *self,
                               DaxElementScript *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_script (self, node);
}

void
dax_traverser_traverse_handler (DaxTraverser      *self,
                                DaxElementHandler *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_handler (self, node);
}

void
dax_traverser_traverse_line (DaxTraverser   *self,
                             DaxElementLine *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_line (self, node);
}

void
dax_traverser_traverse_text (DaxTraverser   *self,
                             DaxElementText *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_text (self, node);
}

void
dax_traverser_traverse_image (DaxTraverser    *self,
                              DaxElementImage *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_image (self, node);
}

void
dax_traverser_traverse_video (DaxTraverser    *self,
                              DaxElementVideo *node)
{
    DaxTraverserClass *klass = DAX_TRAVERSER_GET_CLASS (self);

    klass->traverse_video (self, node);
}
