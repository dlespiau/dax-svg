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

#include "castet-knot-sequence.h"
#include "clutter-shape.h"
#include "castet-build-traverser.h"

G_DEFINE_TYPE (CastetBuildTraverser,
               castet_build_traverser,
               CASTET_TYPE_TRAVERSER)

#define BUILD_TRAVERSER_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      CASTET_TYPE_BUILD_TRAVERSER,  \
                                      CastetBuildTraverserPrivate))

struct _CastetBuildTraverserPrivate
{
    ClutterContainer *container;
    ClutterColor *fill_color;
    ClutterScore *score;
};

static void
castet_build_traverser_traverse_path (CastetTraverser   *traverser,
                                      CastetPathElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    CastetElement *element = CASTET_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *shape;
    ClutterPath *path;

    shape = clutter_shape_new ();

    fill_color = castet_element_get_fill_color (element);
    stroke_color = castet_element_get_stroke_color (element);
    if (fill_color)
        g_object_set (shape, "color", fill_color, NULL);
    if (stroke_color)
        g_object_set (shape, "border-color", stroke_color, NULL);

    g_object_get (node, "d", &path, NULL);
    g_object_set (shape, "path", path, NULL);

    clutter_container_add_actor (priv->container, shape);
}

static void
on_rect_x_changed (CastetRectElement *element,
                   GParamSpec        *pspec,
                   gpointer           user_data)
{
    ClutterActor *rectangle = CLUTTER_ACTOR (user_data);
    gfloat x;

    x = castet_rect_element_get_x_px (element);
    clutter_actor_set_x (rectangle, x);
}

static void
on_rect_y_changed (CastetRectElement *element,
                   GParamSpec        *pspec,
                   gpointer           user_data)
{
    ClutterActor *rectangle = CLUTTER_ACTOR (user_data);
    gfloat y;

    y = castet_rect_element_get_y_px (element);
    clutter_actor_set_y (rectangle, y);
}

static void
on_rect_fill_opacity_changed (CastetRectElement *element,
                              GParamSpec        *pspec,
                              gpointer           user_data)
{
    ClutterRectangle *rectangle = CLUTTER_RECTANGLE (user_data);
    ClutterColor fill_color;
    gfloat fill_opacity;

    fill_opacity = castet_element_get_fill_opacity (CASTET_ELEMENT (element));
    clutter_rectangle_get_color (rectangle, &fill_color);
    fill_color.alpha = fill_opacity * 255;
    clutter_rectangle_set_color (rectangle, &fill_color);
}

static void
castet_build_traverser_traverse_rect (CastetTraverser   *traverser,
                                      CastetRectElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    CastetElement *element = CASTET_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *rectangle;
    ClutterGeometry geom;

    rectangle = clutter_rectangle_new ();

    fill_color = castet_element_get_fill_color (element);
    stroke_color = castet_element_get_stroke_color (element);
    if (fill_color)
        clutter_rectangle_set_color (CLUTTER_RECTANGLE (rectangle),
                                     fill_color);
    if (stroke_color)
        clutter_rectangle_set_border_color (CLUTTER_RECTANGLE (rectangle),
                                            stroke_color);

    /* FIXME castet_rect_element_get_geometry() sounds like it could be
     * useful here */
    geom.x = castet_rect_element_get_x_px (node);
    geom.y = castet_rect_element_get_y_px (node);
    geom.width = castet_rect_element_get_width_px (node);
    geom.height = castet_rect_element_get_height_px (node);
    clutter_actor_set_geometry (rectangle, &geom);

    g_signal_connect (node, "notify::fill-opacity",
                      G_CALLBACK (on_rect_fill_opacity_changed), rectangle);
    g_signal_connect (node, "notify::x",
                      G_CALLBACK (on_rect_x_changed), rectangle);
    g_signal_connect (node, "notify::y",
                      G_CALLBACK (on_rect_y_changed), rectangle);

    clutter_container_add_actor (priv->container, rectangle);
}

static ClutterPath *
clutter_path_new_from_knot_sequence (const CastetKnotSequence *seq)
{
    ClutterPath *path;
    const float *knots;
    guint nb_knots, i;

    nb_knots = castet_knot_sequence_get_size (seq);
    if (nb_knots == 0)
        return NULL;
    knots = castet_knot_sequence_get_array (seq);

    path = clutter_path_new ();
    clutter_path_add_move_to (path, knots[0], knots[1]);

    for (i = 1; i < nb_knots; i++)
        clutter_path_add_line_to (path, knots[i * 2], knots[i * 2 + 1]);

    return path;
}

static void
castet_build_traverser_traverse_polyline (CastetTraverser       *traverser,
                                          CastetPolylineElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    CastetElement *element = CASTET_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *polyline;
    const CastetKnotSequence *seq;
    ClutterPath *path;

    polyline = clutter_shape_new ();

    g_object_get (G_OBJECT (node), "points", &seq, NULL);
    path = clutter_path_new_from_knot_sequence (seq);
    g_object_set (G_OBJECT (polyline), "path", path, NULL);

    fill_color = castet_element_get_fill_color (element);
    stroke_color = castet_element_get_stroke_color (element);
    if (fill_color) {
        g_object_set (polyline, "color", fill_color, NULL);
    }
    if (stroke_color)
        g_object_set (polyline, "border-color", stroke_color, NULL);

    clutter_container_add_actor (priv->container, polyline);
}

static void
_string_to_value (const gchar *string,
                  GValue      *dest)
{
    GValue string_value = { 0, };

    /* we don't want to duplicate the string here */
    g_value_init (&string_value, G_TYPE_STRING);
    g_value_set_static_string (&string_value, string);

    if (g_value_transform (&string_value, dest) == FALSE) {
        g_warning ("Could not transform a string into a %s",
                   G_VALUE_TYPE_NAME (dest));
        return;
    }
}

static void
_animation_setup_property (ClutterAnimation *animation,
                           CastetDomElement *element,
                           const gchar      *property,
                           const gchar      *from,
                           const gchar      *to)
{
    GObjectClass *object_class= G_OBJECT_GET_CLASS (element);
    GParamSpec *pspec;
    GValue from_value = { 0, };
    GValue to_value = { 0, };
    ClutterInterval *interval;

    pspec = g_object_class_find_property (object_class, property);
    if (pspec == NULL) {
        g_message ("Cannot animate property %s: %s has no '%s' attribute",
                   property,
                   G_OBJECT_CLASS_NAME (object_class),
                   property);
        return;
    }

    g_value_init (&from_value, pspec->value_type);
    _string_to_value (from, &from_value);

    g_value_init (&to_value, pspec->value_type);
    _string_to_value (to, &to_value);

    g_message ("setting up interval from %s to %s", from, to);
    interval = clutter_interval_new_with_values (pspec->value_type,
                                                 &from_value,
                                                 &to_value);

    clutter_animation_bind_interval (animation, property, interval);
}

static void
castet_build_traverser_traverse_animate (CastetTraverser      *traverser,
                                         CastetAnimateElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    ClutterAnimation *animation;
    ClutterTimeline *tl;
    CastetDuration *duration;
    const CastetRepeatCount *count;
    CastetDomElement *target_element;
    const gchar *attribute_name, *from, *to;

    /* create a new animation */
    duration = castet_animate_element_get_duration (node);
    target_element = castet_animate_element_get_target (node);
    count = castet_animate_element_get_repeat_count (node);

    animation = clutter_animation_new ();
    clutter_animation_set_duration (animation,
                                    castet_duration_to_ms (duration));
    clutter_animation_set_object (animation, G_OBJECT (target_element));
    clutter_animation_set_mode (animation, CLUTTER_LINEAR);
    if (castet_repeat_count_is_indefinite (count))
        clutter_animation_set_loop (animation, TRUE);

    /* setup the interval */
    attribute_name = castet_animate_element_get_attribute_name (node);
    from = castet_animate_element_get_from (node);
    to = castet_animate_element_get_to (node);
    _animation_setup_property (animation,
                               target_element,
                               attribute_name,
                               from, to);

    /* Add the timeline to the global score */
    tl = clutter_animation_get_timeline (animation);
    clutter_score_append (priv->score, NULL, tl);
}

/*
 * GObject overloading
 */

static void
castet_build_traverser_get_property (GObject    *object,
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
castet_build_traverser_set_property (GObject      *object,
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
castet_build_traverser_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_build_traverser_parent_class)->dispose (object);
}

static void
castet_build_traverser_finalize (GObject *object)
{
    CastetBuildTraverser *self = CASTET_BUILD_TRAVERSER (object);
    CastetBuildTraverserPrivate *priv = self->priv;

    g_object_unref (priv->score);
    g_object_unref (priv->container);

    G_OBJECT_CLASS (castet_build_traverser_parent_class)->finalize (object);
}

static void
castet_build_traverser_class_init (CastetBuildTraverserClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    CastetTraverserClass *traverser_class = CASTET_TRAVERSER_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetBuildTraverserPrivate));

    object_class->get_property = castet_build_traverser_get_property;
    object_class->set_property = castet_build_traverser_set_property;
    object_class->dispose = castet_build_traverser_dispose;
    object_class->finalize = castet_build_traverser_finalize;

    traverser_class->traverse_path = castet_build_traverser_traverse_path;
    traverser_class->traverse_rect = castet_build_traverser_traverse_rect;
    traverser_class->traverse_polyline =
        castet_build_traverser_traverse_polyline;
    traverser_class->traverse_animate = castet_build_traverser_traverse_animate;
}

static void
castet_build_traverser_init (CastetBuildTraverser *self)
{
    CastetBuildTraverserPrivate *priv;

    self->priv = priv = BUILD_TRAVERSER_PRIVATE (self);

    priv->score = clutter_score_new ();
}

CastetTraverser *
castet_build_traverser_new (CastetDomNode    *node,
                            ClutterContainer *container)
{
    CastetTraverser *traverser;

    traverser = g_object_new (CASTET_TYPE_BUILD_TRAVERSER, NULL);
    castet_traverser_set_root (traverser, node);
    castet_build_traverser_set_container (CASTET_BUILD_TRAVERSER (traverser),
                                          container);

    return traverser;
}

void
castet_build_traverser_set_container (CastetBuildTraverser *self,
                                      ClutterContainer     *container)
{
    CastetBuildTraverserPrivate *priv;

    g_return_if_fail (CASTET_IS_BUILD_TRAVERSER (self));
    priv = self->priv;

    if (priv->container)
        g_object_unref (priv->container);
    priv->container = g_object_ref (container);
}

ClutterScore *
castet_build_traverser_get_score (CastetBuildTraverser *self)
{
    g_return_val_if_fail (CASTET_IS_BUILD_TRAVERSER (self), NULL);

    return self->priv->score;
}
