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

#include "dax-utils.h"

#include "dax-shape.h"

G_DEFINE_TYPE (DaxShape, dax_shape, CLUTTER_TYPE_SHAPE)

#define SHAPE_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), DAX_TYPE_SHAPE, DaxShapePrivate))

struct _DaxShapePrivate
{
    CoglMatrix cogl_matrix;
};

/*
 * ClutterActor implementation
 */

static void
dax_shape_apply_transform (ClutterActor *self,
                           CoglMatrix   *matrix)
{
    DaxShape *shape = DAX_SHAPE (self);
    DaxShapePrivate *priv = shape->priv;

    cogl_matrix_multiply (matrix, matrix, &priv->cogl_matrix);
}

/*
 * GObject implementation
 */

static void
dax_shape_class_init (DaxShapeClass *klass)
{
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxShapePrivate));

    actor_class->apply_transform = dax_shape_apply_transform;
}

static void
dax_shape_init (DaxShape *self)
{
    DaxShapePrivate *priv;

    self->priv = priv = SHAPE_PRIVATE (self);
    cogl_matrix_init_identity (&priv->cogl_matrix);
}

ClutterActor *
dax_shape_new (void)
{
    return g_object_new (DAX_TYPE_SHAPE, NULL);
}

void
dax_shape_set_matrix (DaxShape        *self,
                      const DaxMatrix *matrix)
{
    DaxShapePrivate *priv;
    float cogl_matrix[16];

    g_return_if_fail (DAX_IS_SHAPE (self));
    priv = self->priv;

    /* column 0 */
    cogl_matrix[0] = matrix->affine[0];
    cogl_matrix[1] = matrix->affine[1];
    cogl_matrix[2] = 0;
    cogl_matrix[3] = 0;

    /* column 1 */
    cogl_matrix[4] = matrix->affine[2];
    cogl_matrix[5] = matrix->affine[3];
    cogl_matrix[6] = 0;
    cogl_matrix[7] = 0;

    /* column 2 */
    cogl_matrix[8]  = 0;
    cogl_matrix[9]  = 0;
    cogl_matrix[10] = 1;
    cogl_matrix[11] = 0;

    /* column 3 */
    cogl_matrix[12] = matrix->affine[4];
    cogl_matrix[13] = matrix->affine[5];
    cogl_matrix[14] = 0;
    cogl_matrix[15] = 1;

    cogl_matrix_init_from_array (&priv->cogl_matrix, cogl_matrix);

    clutter_actor_queue_redraw (CLUTTER_ACTOR (self));
}
