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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_ACTOR_H__
#define __DAX_ACTOR_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-dom.h"

#include "dax-group.h"

G_BEGIN_DECLS

#define DAX_TYPE_ACTOR dax_actor_get_type()

#define DAX_ACTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ACTOR, DaxActor))

#define DAX_ACTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_ACTOR, DaxActorClass))

#define DAX_IS_ACTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ACTOR))

#define DAX_IS_ACTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ACTOR))

#define DAX_ACTOR_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_ACTOR, DaxActorClass))

typedef struct _DaxActor DaxActor;
typedef struct _DaxActorClass DaxActorClass;
typedef struct _DaxActorPrivate DaxActorPrivate;

struct _DaxActor
{
    DaxGroup parent;

    DaxActorPrivate *priv;
};

struct _DaxActorClass
{
    DaxGroupClass parent_class;
};

GType           dax_actor_get_type          (void) G_GNUC_CONST;

ClutterActor *  dax_actor_new               (void);
ClutterActor *  dax_actor_new_from_file     (const gchar  *filename,
                                             GError      **error);
void            dax_actor_set_document      (DaxActor       *actor,
                                             DaxDomDocument *document);
void            dax_actor_set_playing       (DaxActor *self,
                                             gboolean  playing);

G_END_DECLS

#endif /* __DAX_ACTOR_H__ */
