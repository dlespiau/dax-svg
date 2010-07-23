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
#include "dax-parser.h"
#include "dax-traverser-clutter.h"

#include "dax-actor.h"

G_DEFINE_TYPE (DaxActor, dax_actor, DAX_TYPE_GROUP)

#define ACTOR_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),              \
                                      DAX_TYPE_ACTOR,   \
                                      DaxActorPrivate))

struct _DaxActorPrivate
{
    DaxDomDocument *document;
    ClutterScore *score;
};

static void
remove_actor (ClutterActor *child,
              gpointer      data)
{
    ClutterContainer *self = CLUTTER_CONTAINER (data);

    clutter_container_remove_actor (self, child);
}

static void
dax_actor_rebuild_scene_graph (DaxActor *self)
{
    DaxActorPrivate *priv = self->priv;
    DaxTraverser *traverser;
    DaxTraverserClutter *traverser_clutter;

    /* start by removing everyone */
    clutter_container_foreach (CLUTTER_CONTAINER (self), remove_actor, self);

    traverser = dax_traverser_clutter_new (DAX_DOM_NODE (priv->document),
                                           CLUTTER_CONTAINER (self));
    dax_traverser_apply (traverser);

    traverser_clutter = DAX_TRAVERSER_CLUTTER (traverser);
    priv->score = g_object_ref (dax_traverser_clutter_get_score (traverser_clutter));

    g_object_unref (traverser);
}

/*
 * GObject overloading
 */

static void
dax_actor_get_property (GObject    *object,
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
dax_actor_set_property (GObject      *object,
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
dax_actor_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_actor_parent_class)->dispose (object);
}

static void
dax_actor_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_actor_parent_class)->finalize (object);
}

static void
dax_actor_class_init (DaxActorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxActorPrivate));

    object_class->get_property = dax_actor_get_property;
    object_class->set_property = dax_actor_set_property;
    object_class->dispose = dax_actor_dispose;
    object_class->finalize = dax_actor_finalize;
}

static void
dax_actor_init (DaxActor *self)
{
    self->priv = ACTOR_PRIVATE (self);
}

ClutterActor *
dax_actor_new (void)
{
    return g_object_new (DAX_TYPE_ACTOR, NULL);
}

/* FIXME: filename property, no code in constructors... */
ClutterActor *
dax_actor_new_from_file (const gchar  *filename,
                         GError      **error)
{
    ClutterActor *actor;
    DaxDomDocument *document;

    document = dax_dom_document_new_from_file (filename, error);
    if (document == NULL)
        return NULL;

    actor = dax_actor_new ();
    dax_actor_set_document (DAX_ACTOR (actor), document);

    return actor;
}

void
dax_actor_set_document (DaxActor       *actor,
                        DaxDomDocument *document)
{
    DaxActorPrivate *priv;
    DaxElementSvg *svg;
    ClutterUnits *width, *height;
    DaxMatrix matrix;
    GArray *viewbox;
    double affine[6], scale_x = 1.0, scale_y = 1.0;
    float width_px = 0.f, height_px = 0.f;

    g_return_if_fail (DAX_IS_ACTOR (actor));

    priv = actor->priv;
    priv->document = document;

    dax_actor_rebuild_scene_graph (actor);

    /* set the size of the actor as defined by <svg> width and height */
    svg = DAX_ELEMENT_SVG (dax_dom_document_get_document_element (document));
    width = dax_element_svg_get_width (svg);
    height = dax_element_svg_get_height (svg);
    if (width) {
        width_px = clutter_units_to_pixels (width);
        clutter_actor_set_width(CLUTTER_ACTOR (actor), width_px);
    }
    if (height) {
        height_px = clutter_units_to_pixels (height);
        clutter_actor_set_height(CLUTTER_ACTOR (actor), height_px);
    }

    g_object_get (svg, "viewBox", &viewbox, NULL);
    if (width && height && viewbox) {
        float vb_x, vb_y, vb_width, vb_height;

        vb_x = g_array_index (viewbox, float, 0);
        vb_y = g_array_index (viewbox, float, 1);
        vb_width = g_array_index (viewbox, float, 2);
        vb_height = g_array_index (viewbox, float, 3);

        _dax_affine_identity (affine);
        scale_x = width_px / (vb_width);
        scale_y = height_px / (vb_height);
        _dax_affine_translate (affine, -vb_x, -vb_y);
        _dax_affine_scale (affine, scale_x, scale_y);

        dax_matrix_from_array (&matrix, affine);
        dax_group_set_matrix (DAX_GROUP (actor), &matrix);
    }

    /* FIXME: still something wrong in the size, can't clip just yet... */
#if 0
    if (width && height)
        clutter_actor_set_clip_to_allocation (CLUTTER_ACTOR (actor), TRUE);
#endif

    if (width && height)
        DAX_NOTE (TRANSFORM, "Setting size %.02fx%.02f scale %.02fx%.02f",
                   clutter_units_to_pixels (width),
                   clutter_units_to_pixels (height),
                   scale_x, scale_y);
}

void
dax_actor_set_playing (DaxActor *actor,
                       gboolean  playing)
{
    DaxActorPrivate *priv;

    g_return_if_fail (DAX_IS_ACTOR (actor));
    priv = actor->priv;

    if (playing)
        clutter_score_start (priv->score);
    else
        clutter_score_pause (priv->score);
}
