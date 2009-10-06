/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * Authored by: Øyvind Kolås <pippin@linux.intel.com>
 *              Damien Lespiau <damien.lespiau@intel.com>
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

#include "clutter-shape.h"

#ifndef CLUTTER_PARAM_READWRITE
#define CLUTTER_PARAM_READWRITE \
    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |G_PARAM_STATIC_BLURB
#endif


G_DEFINE_TYPE (ClutterShape, clutter_shape, CLUTTER_TYPE_ACTOR);

enum
{
    PROP_0,
    PROP_COLOR,
    PROP_BORDER_COLOR,
    PROP_PATH,
};

#define CLUTTER_SHAPE_GET_PRIVATE(obj)                  \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj),                \
                                  CLUTTER_TYPE_SHAPE,   \
                                  ClutterShapePrivate))

struct _ClutterShapePrivate
{
    ClutterColor *color;            /* NULL means fill color */
    ClutterColor *border_color;     /* NULL means stroke color */
    ClutterPath *path;
};

static void clutter_path_draw_cogl (const ClutterPathNode *node,
                                    gpointer               user_data)
{
  switch (node->type)
    {
      case CLUTTER_PATH_MOVE_TO:
        cogl_path_move_to (node->points[0].x, node->points[0].y);
        break;
      case CLUTTER_PATH_LINE_TO:
        cogl_path_line_to
          (node->points[0].x, node->points[0].y);
        break;
      case CLUTTER_PATH_CURVE_TO:
        cogl_path_curve_to (node->points[0].x, node->points[0].y,
                            node->points[1].x, node->points[1].y,
                            node->points[2].x, node->points[2].y);
        break;
      case CLUTTER_PATH_REL_MOVE_TO:
        cogl_path_rel_move_to
          (node->points[0].x, node->points[0].y);
        break;
      case CLUTTER_PATH_REL_LINE_TO:
        cogl_path_rel_line_to (node->points[0].x, node->points[0].y);
        break;
      case CLUTTER_PATH_REL_CURVE_TO:
        cogl_path_rel_curve_to
                           (node->points[0].x, node->points[0].y,
                            node->points[1].x, node->points[1].y,
                            node->points[2].x, node->points[2].y);
        break;
      case CLUTTER_PATH_CLOSE:
        cogl_path_close ();
    }
}

static void
clutter_shape_pick (ClutterActor       *self,
                    const ClutterColor *color)
{
  ClutterShape          *shape = CLUTTER_SHAPE(self);
  ClutterShapePrivate *priv = shape->priv;

  if (clutter_actor_should_pick_paint (self))
    {
      cogl_set_source_color4ub (color->red,
                                color->green,
                                color->blue,
                                color->alpha);
      clutter_path_foreach (priv->path, clutter_path_draw_cogl, NULL);
      cogl_path_fill();
    }
}

static void
_apply_opacity_to_color (ClutterActor *self,
                         ClutterColor *color,
                         ClutterColor *blended)
{
    *blended = *color;

    blended->alpha =
        clutter_actor_get_paint_opacity (self) * color->alpha / 255;
}

static void
clutter_shape_paint (ClutterActor *self)
{
  ClutterShape        *shape = CLUTTER_SHAPE(self);
  ClutterShapePrivate *priv = shape->priv;
  ClutterColor         tmp_col;

  clutter_path_foreach (priv->path, clutter_path_draw_cogl, NULL);

  if (priv->color && priv->border_color) {
      _apply_opacity_to_color (self, priv->color, &tmp_col);
      cogl_set_source_color ((CoglColor*)&tmp_col);
      cogl_path_fill_preserve();
  } else if (priv->color) {
      _apply_opacity_to_color (self, priv->color, &tmp_col);
      cogl_set_source_color ((CoglColor*)&tmp_col);
      cogl_path_fill ();
  }

  if (priv->border_color) {
      _apply_opacity_to_color (self, priv->border_color, &tmp_col);
      cogl_set_source_color ((CoglColor*)&tmp_col);
      cogl_path_stroke ();
  }
}

/*
 * GObject overloading
 */

static void
clutter_shape_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    ClutterShape *shape = CLUTTER_SHAPE(object);
    ClutterShapePrivate *priv = shape->priv;

    switch (prop_id)
    {
    case PROP_COLOR:
        g_value_set_boxed (value, priv->color);
        break;
    case PROP_BORDER_COLOR:
        g_value_set_boxed (value, priv->border_color);
        break;
    case PROP_PATH:
        g_value_set_object (value, priv->path);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
clutter_shape_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    ClutterShape *shape = CLUTTER_SHAPE(object);
    ClutterShapePrivate *priv = shape->priv;

    switch (prop_id)
    {
    case PROP_COLOR:
        {
            ClutterColor *color = g_value_get_boxed (value);
            priv->color = clutter_color_copy (color);
            clutter_actor_queue_redraw (CLUTTER_ACTOR (object));
        }
        break;
    case PROP_BORDER_COLOR:
        {
            ClutterColor *color = g_value_get_boxed (value);
            priv->border_color = clutter_color_copy (color);
            clutter_actor_queue_redraw (CLUTTER_ACTOR (object));
        }
        break;
    case PROP_PATH:
        if (priv->path)
            g_object_unref (priv->path);
        priv->path = g_value_get_object (value);
        clutter_actor_queue_redraw (CLUTTER_ACTOR (object));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
clutter_shape_finalize (GObject *object)
{
    ClutterShape          *shape = CLUTTER_SHAPE(object);
    ClutterShapePrivate *priv  = shape->priv;

    if (priv->path)
        g_object_unref (priv->path);

    G_OBJECT_CLASS (clutter_shape_parent_class)->finalize (object);
}

static void
clutter_shape_dispose (GObject *object)
{
    G_OBJECT_CLASS (clutter_shape_parent_class)->dispose (object);
}

static void
clutter_shape_class_init (ClutterShapeClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (gobject_class, sizeof (ClutterShapePrivate));

    actor_class->paint        = clutter_shape_paint;
    actor_class->pick         = clutter_shape_pick;

    gobject_class->finalize     = clutter_shape_finalize;
    gobject_class->dispose      = clutter_shape_dispose;
    gobject_class->set_property = clutter_shape_set_property;
    gobject_class->get_property = clutter_shape_get_property;

    pspec = g_param_spec_boxed ("color",
                                "Color",
                                "The color of the shape",
                                CLUTTER_TYPE_COLOR,
                                CLUTTER_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_COLOR, pspec);

    pspec = g_param_spec_boxed ("border-color",
                                "Border Color",
                                "The color of the outline of the shape",
                                CLUTTER_TYPE_COLOR,
                                CLUTTER_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_BORDER_COLOR, pspec);

    pspec = g_param_spec_object ("path",
                                 "A path describing the shape",
                                 "", /* FIXME */
                                 CLUTTER_TYPE_PATH,
                                 CLUTTER_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_PATH, pspec);
}

static void
clutter_shape_init (ClutterShape *self)
{
    self->priv = CLUTTER_SHAPE_GET_PRIVATE (self);
}

/**
 * clutter_shape_new:
 *
 * Creates a new #ClutterActor with a rectangular shape.
 *
 * Return value: a new #ClutterActor
 */
ClutterActor*
clutter_shape_new (void)
{
    return g_object_new (CLUTTER_TYPE_SHAPE, NULL);
}
