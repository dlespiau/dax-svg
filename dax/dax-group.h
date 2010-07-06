/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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

#ifndef __DAX_GROUP_H__
#define __DAX_GROUP_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-types.h"

G_BEGIN_DECLS

#define DAX_TYPE_GROUP dax_group_get_type()

#define DAX_GROUP(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),  DAX_TYPE_GROUP, DaxGroup))

#define DAX_GROUP_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_GROUP, DaxGroupClass))

#define DAX_IS_GROUP(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_GROUP))

#define DAX_IS_GROUP_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_GROUP))

#define DAX_GROUP_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_GROUP, DaxGroupClass))

typedef struct _DaxGroup DaxGroup;
typedef struct _DaxGroupClass DaxGroupClass;
typedef struct _DaxGroupPrivate DaxGroupPrivate;

struct _DaxGroup
{
    ClutterGroup parent;

    DaxGroupPrivate *priv;
};

struct _DaxGroupClass
{
    ClutterGroupClass parent_class;
};

GType           dax_group_get_type      (void) G_GNUC_CONST;

ClutterActor *  dax_group_new           (void);
void            dax_group_set_matrix    (DaxGroup        *self,
                                         const DaxMatrix *matrix);

G_END_DECLS

#endif /* __DAX_GROUP_H__ */
