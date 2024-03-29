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

#include <string.h>

#include <clutter-gst/clutter-gst.h>

#include "dax-dom.h"

#include "clutter-shape.h"
#include "dax-debug.h"
#include "dax-enum-types.h"
#include "dax-group.h"
#include "dax-internals.h"
#include "dax-knot-sequence.h"
#include "dax-shape.h"
#include "dax-utils.h"

#include "dax-traverser-clutter.h"

G_DEFINE_TYPE (DaxTraverserClutter,
               dax_traverser_clutter,
               DAX_TYPE_TRAVERSER)

#define TRAVERSER_CLUTTER_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      DAX_TYPE_TRAVERSER_CLUTTER,   \
                                      DaxTraverserClutterPrivate))

static GQuark quark_object_actor;

enum
{
    PROP_0,

    PROP_CONTAINER
};

struct _DaxTraverserClutterPrivate
{
    ClutterContainer *container;
    ClutterColor *fill_color;
    ClutterScore *score;
    GPtrArray *media;               /* Array of ClutterMedia objects */
};

static void
xml_event_from_clutter_event (DaxXmlEvent       *xml_event,
                              ClutterEvent      *clutter_event,
                              DaxXmlEventTarget *target)
{
    switch (clutter_event->type) {
    case CLUTTER_BUTTON_RELEASE:
    {
        DaxXmlMouseEvent *event = &xml_event->mouse_event;

        event->type = DAX_XML_EVENT_TYPE_CLICK;
        event->target = g_object_ref (target);
    }
        break;
    case CLUTTER_NOTHING:
    case CLUTTER_KEY_PRESS:
    case CLUTTER_KEY_RELEASE:
    case CLUTTER_MOTION:
    case CLUTTER_ENTER:
    case CLUTTER_LEAVE:
    case CLUTTER_BUTTON_PRESS:
    case CLUTTER_SCROLL:
    case CLUTTER_STAGE_STATE:
    case CLUTTER_DESTROY_NOTIFY:
    case CLUTTER_CLIENT_MESSAGE:
    case CLUTTER_DELETE:
    default:
        g_warning (G_STRLOC ": Unhandled event of type %d",
                   clutter_event->type);
    }
}

static void
set_container_internal (DaxTraverserClutter *self,
                        ClutterContainer    *container)
{
    DaxTraverserClutterPrivate *priv = self->priv;

    if (priv->container)
        g_object_unref (priv->container);
    priv->container = g_object_ref (container);
}

static void
on_g_transform_changed (DaxElementG *element,
                        GParamSpec  *pspec,
                        gpointer     user_data)
{
    DaxGroup *group = DAX_GROUP (user_data);
    const DaxMatrix *matrix;

    matrix = dax_element_g_get_transform (element);
    dax_group_set_matrix (group, matrix);
}

static void
dax_traverser_clutter_traverse_g (DaxTraverser    *traverser,
                                  DaxElementG     *node,
                                  DaxTraverserWay  way)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    ClutterActor *group;
    DaxMatrix *matrix;

    if (way == DAX_TRAVERSER_WAY_END) {
        ClutterActor *parent;

        parent = clutter_actor_get_parent (CLUTTER_ACTOR (priv->container));
        set_container_internal (build, CLUTTER_CONTAINER (parent));
        return;
    }

    group = dax_group_new ();
    clutter_container_add_actor (priv->container, group);
    set_container_internal (build, CLUTTER_CONTAINER (group));

    g_signal_connect (node, "notify::transform",
                      G_CALLBACK (on_g_transform_changed), group);

    g_object_get (node, "transform", &matrix, NULL);
    if (matrix == NULL)
        return;

    dax_group_set_matrix (DAX_GROUP (group), matrix);
    dax_matrix_free (matrix);
}

static void
on_path_transform_changed (DaxElementPath *element,
                           GParamSpec     *pspec,
                           gpointer        user_data)
{
    DaxShape *shape = DAX_SHAPE (user_data);
    const DaxMatrix *matrix;

    matrix = dax_element_path_get_transform (element);
    dax_shape_set_matrix (shape, matrix);
}

static void
dax_traverser_clutter_traverse_path (DaxTraverser   *traverser,
                                     DaxElementPath *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElement *element = DAX_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *shape;
    ClutterPath2D *path;
    DaxMatrix *matrix;

    shape = dax_shape_new ();

    fill_color = dax_element_get_fill_color (element);
    stroke_color = dax_element_get_stroke_color (element);
    if (fill_color)
        g_object_set (shape, "color", fill_color, NULL);
    if (stroke_color)
        g_object_set (shape, "border-color", stroke_color, NULL);

    g_object_get (node, "d", &path, NULL);
    g_object_set (shape, "path", path, NULL);

    clutter_container_add_actor (priv->container, shape);

    g_signal_connect (node, "notify::transform",
                      G_CALLBACK (on_path_transform_changed), shape);

    g_object_get (node, "transform", &matrix, NULL);
    if (matrix == NULL)
        return;

    dax_shape_set_matrix (DAX_SHAPE (shape), matrix);
    dax_matrix_free (matrix);
}

static void
on_rect_x_changed (DaxElementRect *element,
                   GParamSpec     *pspec,
                   gpointer        user_data)
{
    ClutterActor *rectangle = CLUTTER_ACTOR (user_data);
    gfloat x;

    x = dax_element_rect_get_x_px (element);
    clutter_actor_set_x (rectangle, x);
}

static void
on_rect_y_changed (DaxElementRect *element,
                   GParamSpec     *pspec,
                   gpointer        user_data)
{
    ClutterActor *rectangle = CLUTTER_ACTOR (user_data);
    gfloat y;

    y = dax_element_rect_get_y_px (element);
    clutter_actor_set_y (rectangle, y);
}

static void
on_rect_fill_opacity_changed (DaxElementRect *element,
                              GParamSpec     *pspec,
                              gpointer        user_data)
{
    ClutterRectangle *rectangle = CLUTTER_RECTANGLE (user_data);
    ClutterColor fill_color;
    gfloat fill_opacity;

    fill_opacity = dax_element_get_fill_opacity (DAX_ELEMENT (element));
    clutter_rectangle_get_color (rectangle, &fill_color);
    fill_color.alpha = fill_opacity * 255;
    clutter_rectangle_set_color (rectangle, &fill_color);
}

static void
dax_traverser_clutter_traverse_rect (DaxTraverser   *traverser,
                                     DaxElementRect *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElement *element = DAX_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *rectangle;
    ClutterGeometry geom;

    rectangle = clutter_rectangle_new ();

    fill_color = dax_element_get_fill_color (element);
    stroke_color = dax_element_get_stroke_color (element);
    if (fill_color)
        clutter_rectangle_set_color (CLUTTER_RECTANGLE (rectangle),
                                     fill_color);
    if (stroke_color)
        clutter_rectangle_set_border_color (CLUTTER_RECTANGLE (rectangle),
                                            stroke_color);

    /* FIXME dax_element_rect_get_geometry() sounds like it could be
     * useful here */
    geom.x = dax_element_rect_get_x_px (node);
    geom.y = dax_element_rect_get_y_px (node);
    geom.width = dax_element_rect_get_width_px (node);
    geom.height = dax_element_rect_get_height_px (node);
    clutter_actor_set_geometry (rectangle, &geom);

    g_signal_connect (node, "notify::fill-opacity",
                      G_CALLBACK (on_rect_fill_opacity_changed), rectangle);
    g_signal_connect (node, "notify::x",
                      G_CALLBACK (on_rect_x_changed), rectangle);
    g_signal_connect (node, "notify::y",
                      G_CALLBACK (on_rect_y_changed), rectangle);

    clutter_container_add_actor (priv->container, rectangle);
}

static ClutterPath2D *
clutter_path_new_from_knot_sequence (const DaxKnotSequence *seq)
{
    ClutterPath2D *path;
    const float *knots;
    guint nb_knots, i;

    nb_knots = dax_knot_sequence_get_size (seq);
    if (nb_knots == 0)
        return NULL;
    knots = dax_knot_sequence_get_array (seq);

    path = clutter_path_2d_new ();
    clutter_path_2d_add_move_to (path, knots[0], knots[1]);

    for (i = 1; i < nb_knots; i++)
        clutter_path_2d_add_line_to (path, knots[i * 2], knots[i * 2 + 1]);

    return path;
}

static void
dax_traverser_clutter_traverse_polyline (DaxTraverser       *traverser,
                                         DaxElementPolyline *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElement *element = DAX_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *polyline;
    const DaxKnotSequence *seq;
    ClutterPath2D *path;

    polyline = clutter_shape_new ();

    g_object_get (G_OBJECT (node), "points", &seq, NULL);
    path = clutter_path_new_from_knot_sequence (seq);
    g_object_set (G_OBJECT (polyline), "path", path, NULL);

    fill_color = dax_element_get_fill_color (element);
    stroke_color = dax_element_get_stroke_color (element);
    if (fill_color) {
        g_object_set (polyline, "color", fill_color, NULL);
    }
    if (stroke_color)
        g_object_set (polyline, "border-color", stroke_color, NULL);

    clutter_container_add_actor (priv->container, polyline);
}

static ClutterPath2D *
build_circle_path (DaxElementCircle *circle)
{
    ClutterPath2D *path;
    ClutterUnits *cx_u, *cy_u, *r_u;
    gfloat cx, cy, r;
    static gfloat k = 4 * (G_SQRT2 - 1) / 3;

    /* Build the ClutterPath2D */
    cx_u = dax_element_circle_get_cx (circle);
    cy_u = dax_element_circle_get_cy (circle);
    r_u  = dax_element_circle_get_r (circle);
    cx = clutter_units_to_pixels (cx_u);
    cy = clutter_units_to_pixels (cy_u);
    r = clutter_units_to_pixels (r_u);

    path = clutter_path_2d_new ();

    clutter_path_2d_add_move_to (path, cx + r, cy);
    clutter_path_2d_add_curve_to (path,
                                  cx + r, cy + r * k,
                                  cx + r * k, cy + r,
                                  cx, cy + r);
    clutter_path_2d_add_curve_to (path,
                                  cx - r * k , cy + r,
                                  cx - r , cy + r * k,
                                  cx - r, cy);
    clutter_path_2d_add_curve_to (path,
                                  cx - r, cy - r * k,
                                  cx - r * k, cy - r,
                                  cx, cy - r);
    clutter_path_2d_add_curve_to (path,
                                  cx + r * k, cy - r,
                                  cx + r, cy - r * k,
                                  cx + r, cy);
    clutter_path_2d_add_close (path);

    return path;
}

static void
on_circle_changed (DaxElementCircle *circle,
                   GParamSpec       *pspec,
                   ClutterActor     *target)
{
    ClutterPath2D *path;

    path = build_circle_path (circle);
    g_object_set (target, "path", path, NULL);
}

static void
dax_traverser_clutter_traverse_circle (DaxTraverser     *traverser,
                                       DaxElementCircle *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElement *element = DAX_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *circle;
    ClutterPath2D *path;

    path = build_circle_path (node);

    circle = clutter_shape_new ();
    g_object_set (circle, "path", path, NULL);

    /* handle fill / stroke colors */
    fill_color = dax_element_get_fill_color (element);
    stroke_color = dax_element_get_stroke_color (element);
    if (fill_color) {
        g_object_set (circle, "color", fill_color, NULL);
    }
    if (stroke_color)
        g_object_set (circle, "border-color", stroke_color, NULL);

    g_object_set_qdata (G_OBJECT (node), quark_object_actor, circle);
    g_signal_connect (node, "notify::r",
                      G_CALLBACK (on_circle_changed), circle);
    clutter_container_add_actor (priv->container, circle);
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
                           DaxDomElement    *element,
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
        g_warning ("Cannot animate property %s: %s has no '%s' attribute",
                   property,
                   G_OBJECT_CLASS_NAME (object_class),
                   property);
        return;
    }

    g_value_init (&from_value, pspec->value_type);
    _string_to_value (from, &from_value);

    g_value_init (&to_value, pspec->value_type);
    _string_to_value (to, &to_value);

    DAX_NOTE (ANIMATION, "setting up animation of %s on %s with interval "
              "going from %s to %s", property, G_OBJECT_TYPE_NAME (element),
              from, to);

    interval = clutter_interval_new_with_values (pspec->value_type,
                                                 &from_value,
                                                 &to_value);

    clutter_animation_bind_interval (animation, property, interval);
}

static ClutterAnimation *
_clutter_animation_new_from_dax_animation (DaxElementAnimation *node,
                                           DaxDomElement       *target_element)
{
    ClutterAnimation *animation;
    DaxDuration *duration;
    const DaxRepeatCount *count;

    /* create a new animation */
    duration = dax_element_animation_get_duration (node);
    count = dax_element_animation_get_repeat_count (node);

    animation = clutter_animation_new ();
    clutter_animation_set_duration (animation,
                                    dax_duration_to_ms (duration));
    clutter_animation_set_object (animation, G_OBJECT (target_element));
    clutter_animation_set_mode (animation, CLUTTER_LINEAR);
    if (dax_repeat_count_is_indefinite (count))
        clutter_animation_set_loop (animation, TRUE);

    return animation;
}

static void
dax_traverser_clutter_traverse_animate (DaxTraverser      *traverser,
                                        DaxElementAnimate *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElementAnimation *animation_node = DAX_ELEMENT_ANIMATION (node);
    const gchar *attribute_name, *from, *to;
    DaxDomElement *target_element;
    ClutterAnimation *animation;
    ClutterTimeline *tl;

    target_element = dax_element_animation_get_target (animation_node);
    animation = _clutter_animation_new_from_dax_animation (animation_node,
                                                           target_element);

    /* setup the interval */
    attribute_name = dax_element_animation_get_attribute_name (animation_node);
    from = dax_element_animation_get_from (animation_node);
    to = dax_element_animation_get_to (animation_node);
    _animation_setup_property (animation,
                               target_element,
                               attribute_name,
                               from, to);

    /* Add the timeline to the global score */
    tl = clutter_animation_get_timeline (animation);
    clutter_score_append (priv->score, NULL, tl);
}

static void
dax_traverser_clutter_traverse_animate_transform (
    DaxTraverser               *traverser,
    DaxElementAnimateTransform *node
)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElementAnimation *animation_node = DAX_ELEMENT_ANIMATION (node);
    const gchar *attribute_name, *from, *to;
    DaxDomElement *target_element;
    DaxAnimateTransformType type;
    ClutterAnimation *animation;
    ClutterTimeline *tl;

    target_element = dax_element_animation_get_target (animation_node);
    animation = _clutter_animation_new_from_dax_animation (animation_node,
                                                           target_element);

    /* setup the interval */
    type = dax_element_animate_transform_get_matrix_type (node);
    attribute_name = dax_element_animation_get_attribute_name (animation_node);
    from =
        g_strdup_printf("%s(%s)",
                        dax_enum_to_string (DAX_TYPE_ANIMATE_TRANSFORM_TYPE,
                                            type),
                        dax_element_animation_get_from (animation_node));
    to =
        g_strdup_printf("%s(%s)",
                        dax_enum_to_string (DAX_TYPE_ANIMATE_TRANSFORM_TYPE,
                                            type),
                        dax_element_animation_get_to (animation_node));
    _animation_setup_property (animation,
                               target_element,
                               attribute_name,
                               from, to);

    /* Add the timeline to the global score */
    tl = clutter_animation_get_timeline (animation);
    clutter_score_append (priv->score, NULL, tl);
}

static gboolean
on_button_release_event (ClutterActor *actor,
                         ClutterEvent *event,
                         gpointer      user_data)
{
    DaxXmlEvent xml_event;
    DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (user_data);

    xml_event_from_clutter_event (&xml_event, event, target);

    dax_xml_event_target_handle_event (target, dax_xml_event_copy (&xml_event));

    return TRUE;
}

static void
on_load_event (DaxDomElement *element,
               gboolean       loaded,
               gpointer       user_data)
{
    DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (element);
    DaxXmlEvent load_event;

    dax_xml_event_from_type (&load_event, DAX_XML_EVENT_TYPE_LOAD, target);

    dax_xml_event_target_handle_event (target,
                                       dax_xml_event_copy (&load_event));
}

static gboolean event_needs_reactive (DaxXmlEventType type)
{
    return type >= DAX_XML_EVENT_TYPE_FIRST_MOUSE_EVENT &&
           type <= DAX_XML_EVENT_TYPE_LAST_MOUSE_EVENT; 
}

static void
dax_traverser_clutter_traverse_handler (DaxTraverser      *traverser,
                                        DaxElementHandler *node)
{
#if 0
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
#endif
    DaxDomElement *target;
    ClutterActor *target_actor;
    DaxXmlEventTarget *event_target;
    DaxXmlEventListener *event_listener;
    DaxXmlEventType event_type;
    const gchar *event_name;

    target = dax_element_handler_get_target (node);

    g_object_get (node, "event", &event_type, NULL);
    event_name = dax_enum_to_string (DAX_TYPE_XML_EVENT_TYPE, event_type);

    event_target = DAX_XML_EVENT_TARGET (target);
    event_listener = DAX_XML_EVENT_LISTENER (node);

    dax_xml_event_target_add_event_listener (event_target,
                                             event_name,
                                             event_listener,
                                             FALSE);

    /* if the observer is bound to an actor and the event watched requires the
     * actor to be reactive, so be it */
    target_actor = g_object_get_qdata (G_OBJECT (target), quark_object_actor);
    if (target_actor && event_needs_reactive (event_type)) {
        clutter_actor_set_reactive (target_actor, TRUE);
    }

    switch (event_type) {
    case DAX_XML_EVENT_TYPE_CLICK:
        if (G_UNLIKELY (target_actor == NULL)) {
            g_warning (G_STRLOC ": Target has no ClutterActor bound to it");
            break;
        }
        g_signal_connect (target_actor, "button-release-event",
                          G_CALLBACK (on_button_release_event), target);
        break;
    case DAX_XML_EVENT_TYPE_LOAD:
        if (dax_dom_element_is_loaded (target)) {
            on_load_event (target, TRUE, NULL);
        } else  {
            g_signal_connect (target, "loaded",
                              G_CALLBACK (on_load_event), NULL);
        }
        break;

    case DAX_XML_EVENT_TYPE_SVG_TIMER:
    case DAX_XML_EVENT_TYPE_NONE:
    default:
        g_warning (G_STRLOC ": Unkown event %d", event_type);
    }
}

static ClutterPath2D *
clutter_path_new_from_line (DaxElementLine *line)
{
    ClutterUnits *x1_u, *y1_u, *x2_u, *y2_u;
    gfloat x1, y1, x2, y2;
    ClutterPath2D *path;

    x1_u = dax_element_line_get_x1 (line);
    y1_u = dax_element_line_get_y1 (line);
    x2_u = dax_element_line_get_x2 (line);
    y2_u = dax_element_line_get_y2 (line);

    x1 = clutter_units_to_pixels (x1_u);
    y1 = clutter_units_to_pixels (y1_u);
    x2 = clutter_units_to_pixels (x2_u);
    y2 = clutter_units_to_pixels (y2_u);

    path = clutter_path_2d_new ();
    clutter_path_2d_add_move_to (path, x1, y1);
    clutter_path_2d_add_line_to (path, x2, y2);

    return path;
}

static void
dax_traverser_clutter_traverse_line (DaxTraverser   *traverser,
                                     DaxElementLine *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    DaxElement *element = DAX_ELEMENT (node);
    const ClutterColor *stroke_color;
    ClutterActor *line;
    ClutterPath2D *path;

    line = clutter_shape_new ();
    path = clutter_path_new_from_line (node);
    g_object_set (G_OBJECT (line), "path", path, NULL);

    stroke_color = dax_element_get_stroke_color (element);
    if (stroke_color)
        g_object_set (line, "border-color", stroke_color, NULL);

    clutter_container_add_actor (priv->container, line);
}

static ClutterActor *
clutter_text_new_from_dax_text (DaxElementText *text)
{
    ClutterActor *text_actor;
    GArray *xs, *ys;
    ClutterUnits *x_u, *y_u;
    gfloat x, y;
    gchar *flatten_text;
    PangoLayout *layout;
    gchar *font_family, *font_size;
    GString *font_name;

    xs = dax_element_text_get_x (text);
    ys = dax_element_text_get_y (text);

    x_u = &g_array_index (xs, ClutterUnits, 0);
    y_u = &g_array_index (ys, ClutterUnits, 0);

    x = clutter_units_to_pixels (x_u);
    y = clutter_units_to_pixels (y_u);

    flatten_text = dax_element_text_get_text (text);

    text_actor = clutter_text_new ();
    clutter_text_set_text (CLUTTER_TEXT (text_actor), flatten_text);

    /* font name. FIXME: inherit the properties */
    g_object_get (text,
                  "font-family", &font_family,
                  "font-size", &font_size,
                  NULL);
    font_name = g_string_new ("");
    if (font_family)
        g_string_append (font_name, font_family);
    if (font_size) {
        g_string_append_c (font_name, ' ');
        g_string_append (font_name, font_size);
    }
    if (font_name->len > 1)
        clutter_text_set_font_name (CLUTTER_TEXT (text_actor), font_name->str);
    g_string_free (font_name, TRUE);

    /* SVG text is position relatively to its baseline */
    layout = clutter_text_get_layout (CLUTTER_TEXT (text_actor));
    clutter_actor_set_position (text_actor,
                                x,
                                y  - pango_layout_get_baseline (layout) / 1024);

    return text_actor;
}

static void
dax_traverser_clutter_traverse_text (DaxTraverser   *traverser,
                                     DaxElementText *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    ClutterActor *text;

    text = clutter_text_new_from_dax_text (node);

    clutter_container_add_actor (priv->container, text);
}

static ClutterActor *
clutter_texture_new_from_dax_image (DaxElementImage *image)
{
    ClutterActor *actor;
    ClutterUnits *x_u, *y_u, *width_u, *height_u;
    gfloat x, y, width, height;
    const DaxCacheEntry *entry;
    gchar *path;

    x_u = dax_element_image_get_x (image);
    y_u = dax_element_image_get_y (image);
    width_u = dax_element_image_get_width (image);
    height_u = dax_element_image_get_height (image);

    x = clutter_units_to_pixels (x_u);
    y = clutter_units_to_pixels (y_u);
    width = clutter_units_to_pixels (width_u);
    height = clutter_units_to_pixels (height_u);

    entry = dax_element_image_get_cache_entry (image);
    path = dax_cache_entry_get_local_path (entry);

    /* FIXME: should be async */
    actor = clutter_texture_new_from_file (path, NULL);
    clutter_actor_set_x (actor, x);
    clutter_actor_set_y (actor, x);
    clutter_actor_set_width (actor, width);
    clutter_actor_set_height (actor, height);

    return actor;
}

static void
on_image_loaded (DaxElementImage     *image,
                 gboolean             loaded,
                 DaxTraverserClutter *traverver)
{
    DaxTraverserClutterPrivate *priv = traverver->priv;
    ClutterActor *actor;

    if (loaded == FALSE)
        return;

    actor = clutter_texture_new_from_dax_image (image);
    clutter_container_add_actor (priv->container, actor);
}

static void
dax_traverser_clutter_traverse_image (DaxTraverser    *traverser,
                                      DaxElementImage *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);

    if (dax_dom_element_is_loaded (DAX_DOM_ELEMENT (node)))
        on_image_loaded (node, TRUE, build);
    g_signal_connect (node, "loaded", G_CALLBACK (on_image_loaded), build);
}


static void
dax_traverser_clutter_traverse_video (DaxTraverser    *traverser,
                                      DaxElementVideo *node)
{
    DaxTraverserClutter *build = DAX_TRAVERSER_CLUTTER (traverser);
    DaxTraverserClutterPrivate *priv = build->priv;
    ClutterActor *video;
    ClutterUnits *x_u, *y_u, *width_u, *height_u;
    gfloat x, y, width, height;
    const gchar *uri;

    x_u = dax_element_video_get_x (node);
    y_u = dax_element_video_get_y (node);
    width_u = dax_element_video_get_width (node);
    height_u = dax_element_video_get_height (node);

    x = clutter_units_to_pixels (x_u);
    y = clutter_units_to_pixels (y_u);
    width = clutter_units_to_pixels (width_u);
    height = clutter_units_to_pixels (height_u);

    video = clutter_gst_video_texture_new ();
    clutter_actor_set_x (video, x);
    clutter_actor_set_y (video, x);
    clutter_actor_set_width (video, width);
    clutter_actor_set_height (video, height);

    uri = dax_element_video_get_uri (node);
    clutter_media_set_uri (CLUTTER_MEDIA (video), uri);
    g_ptr_array_add (priv->media, video);

    clutter_container_add_actor (priv->container, video);
}

/*
 * GObject implementation
 */

static void
dax_traverser_clutter_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    DaxTraverserClutter *traverser = DAX_TRAVERSER_CLUTTER (object);
    DaxTraverserClutterPrivate *priv =  traverser->priv;

    switch (property_id)
    {
    case PROP_CONTAINER:
        g_value_set_object (value, priv->container);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_traverser_clutter_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    DaxTraverserClutter *traverser = DAX_TRAVERSER_CLUTTER (object);

    switch (property_id)
    {
    case PROP_CONTAINER:
        set_container_internal (traverser, g_value_get_object (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_traverser_clutter_dispose (GObject *object)
{
    DaxTraverserClutter *traverser = DAX_TRAVERSER_CLUTTER (object);
    DaxTraverserClutterPrivate *priv = traverser->priv;

    if (priv->container) {
        g_object_unref (priv->container);
        priv->container = NULL;
    }

    G_OBJECT_CLASS (dax_traverser_clutter_parent_class)->dispose (object);
}

static void
dax_traverser_clutter_finalize (GObject *object)
{
    DaxTraverserClutter *self = DAX_TRAVERSER_CLUTTER (object);
    DaxTraverserClutterPrivate *priv = self->priv;

    g_object_unref (priv->score);

    G_OBJECT_CLASS (dax_traverser_clutter_parent_class)->finalize (object);
}

static void
dax_traverser_clutter_class_init (DaxTraverserClutterClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxTraverserClass *traverser_class = DAX_TRAVERSER_CLASS (klass);
    GParamSpec *pspec;

    quark_object_actor = g_quark_from_static_string ("dax-clutter-actor");

    g_type_class_add_private (klass, sizeof (DaxTraverserClutterPrivate));

    object_class->get_property = dax_traverser_clutter_get_property;
    object_class->set_property = dax_traverser_clutter_set_property;
    object_class->dispose = dax_traverser_clutter_dispose;
    object_class->finalize = dax_traverser_clutter_finalize;

    traverser_class->traverse_g = dax_traverser_clutter_traverse_g;
    traverser_class->traverse_path = dax_traverser_clutter_traverse_path;
    traverser_class->traverse_rect = dax_traverser_clutter_traverse_rect;
    traverser_class->traverse_polyline =
        dax_traverser_clutter_traverse_polyline;
    traverser_class->traverse_animate = dax_traverser_clutter_traverse_animate;
    traverser_class->traverse_animate_transform =
        dax_traverser_clutter_traverse_animate_transform;
    traverser_class->traverse_circle = dax_traverser_clutter_traverse_circle;
    traverser_class->traverse_handler = dax_traverser_clutter_traverse_handler;
    traverser_class->traverse_line = dax_traverser_clutter_traverse_line;
    traverser_class->traverse_text = dax_traverser_clutter_traverse_text;
    traverser_class->traverse_image = dax_traverser_clutter_traverse_image;
    traverser_class->traverse_video = dax_traverser_clutter_traverse_video;

    pspec = g_param_spec_object ("container",
                                 "Container",
                                 "Container where to put the ClutterActors",
                                 CLUTTER_TYPE_CONTAINER,
                                 DAX_GPARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (object_class, PROP_CONTAINER, pspec);
}

static void
dax_traverser_clutter_init (DaxTraverserClutter *self)
{
    DaxTraverserClutterPrivate *priv;

    self->priv = priv = TRAVERSER_CLUTTER_PRIVATE (self);

    priv->score = clutter_score_new ();
    priv->media = g_ptr_array_new ();
}

DaxTraverser *
dax_traverser_clutter_new (DaxDomNode       *root,
                           ClutterContainer *container)
{
    return g_object_new (DAX_TYPE_TRAVERSER_CLUTTER,
                         "root", root,
                         "container", container,
                         NULL);
}

ClutterScore *
dax_traverser_clutter_get_score (DaxTraverserClutter *self)
{
    g_return_val_if_fail (DAX_IS_TRAVERSER_CLUTTER (self), NULL);

    return self->priv->score;
}

GPtrArray *
dax_traverser_clutter_get_media (DaxTraverserClutter *self)
{
    g_return_val_if_fail (DAX_IS_TRAVERSER_CLUTTER (self), NULL);

    return self->priv->media;
}
