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

#include "castet-build-traverser.h"
#include "castet-parser.h"
#include "castet-actor.h"

G_DEFINE_TYPE (CastetActor, castet_actor, CLUTTER_TYPE_GROUP)

#define ACTOR_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      CASTET_TYPE_ACTOR,    \
                                      CastetActorPrivate))

struct _CastetActorPrivate
{
    CastetDomDocument *document;
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
castet_actor_rebuild_scene_graph (CastetActor *self)
{
    CastetActorPrivate *priv = self->priv;
    CastetTraverser *traverser;
    CastetBuildTraverser *build_traverser;

    /* start by removing everyone */
    clutter_container_foreach (CLUTTER_CONTAINER (self), remove_actor, self);

    traverser = castet_build_traverser_new (CASTET_DOM_NODE (priv->document),
                                            CLUTTER_CONTAINER (self));
    castet_traverser_apply (traverser);

    build_traverser = CASTET_BUILD_TRAVERSER (traverser);
    priv->score = g_object_ref (castet_build_traverser_get_score (build_traverser));

    g_object_unref (traverser);
}

/*
 * GObject overloading
 */

static void
castet_actor_get_property (GObject    *object,
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
castet_actor_set_property (GObject      *object,
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
castet_actor_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_actor_parent_class)->dispose (object);
}

static void
castet_actor_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_actor_parent_class)->finalize (object);
}

static void
castet_actor_class_init (CastetActorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetActorPrivate));

    object_class->get_property = castet_actor_get_property;
    object_class->set_property = castet_actor_set_property;
    object_class->dispose = castet_actor_dispose;
    object_class->finalize = castet_actor_finalize;
}

static void
castet_actor_init (CastetActor *self)
{
    self->priv = ACTOR_PRIVATE (self);
}

ClutterActor *
castet_actor_new (void)
{
    return g_object_new (CASTET_TYPE_ACTOR, NULL);
}

ClutterActor *
castet_actor_new_from_file (const gchar  *filename,
                            GError      **error)
{
    ClutterActor *actor;
    CastetDomDocument *document;

    document = castet_dom_document_new_from_file (filename, error);
    if (document == NULL)
        return NULL;

    actor = castet_actor_new ();
    castet_actor_set_document (CASTET_ACTOR (actor), document);

    return actor;
}

void
castet_actor_set_document (CastetActor       *actor,
                           CastetDomDocument *document)
{
    CastetActorPrivate *priv;

    g_return_if_fail (CASTET_IS_ACTOR (actor));

    priv = actor->priv;
    priv->document = document;

    castet_actor_rebuild_scene_graph (actor);
}

/* FIXME a real play/pause/stop api or hand back the ClutterScore ? */
void
castet_actor_play (CastetActor *actor)
{
    g_return_if_fail (CASTET_IS_ACTOR (actor));

    clutter_score_start (actor->priv->score);
}
