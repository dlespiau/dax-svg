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

#ifndef __CASTET_ACTOR_H__
#define __CASTET_ACTOR_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "castet-dom-document.h"

G_BEGIN_DECLS

#define CASTET_TYPE_ACTOR castet_actor_get_type()

#define CASTET_ACTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), CASTET_TYPE_ACTOR, CastetActor))

#define CASTET_ACTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), CASTET_TYPE_ACTOR, CastetActorClass))

#define CASTET_IS_ACTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_ACTOR))

#define CASTET_IS_ACTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_ACTOR))

#define CASTET_ACTOR_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), CASTET_TYPE_ACTOR, CastetActorClass))

typedef struct _CastetActor CastetActor;
typedef struct _CastetActorClass CastetActorClass;
typedef struct _CastetActorPrivate CastetActorPrivate;

struct _CastetActor
{
    ClutterGroup parent;

    CastetActorPrivate *priv;
};

struct _CastetActorClass
{
    ClutterGroupClass parent_class;
};

GType           castet_actor_get_type           (void) G_GNUC_CONST;

ClutterActor   *castet_actor_new                (void);
ClutterActor   *castet_actor_new_from_file      (const gchar  *filename,
                                                 GError      **error);
void            castet_actor_set_document       (CastetActor       *actor,
                                                 CastetDomDocument *document);
void            castet_actor_play               (CastetActor *self);

G_END_DECLS

#endif /* __CASTET_ACTOR_H__ */
