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

#include "dax-traverser-clutter.h"
#include "dax-parser.h"
#include "dax-actor.h"

G_DEFINE_TYPE (DaxActor, dax_actor, CLUTTER_TYPE_GROUP)

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

    g_return_if_fail (DAX_IS_ACTOR (actor));

    priv = actor->priv;
    priv->document = document;

    dax_actor_rebuild_scene_graph (actor);
}

/* FIXME a real play/pause/stop api or hand back the ClutterScore ? */
void
dax_actor_play (DaxActor *actor)
{
    g_return_if_fail (DAX_IS_ACTOR (actor));

    clutter_score_start (actor->priv->score);
}
