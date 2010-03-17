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

#include <string.h>

#include "castet-enum-types.h"
#include "castet-js-context.h"
#include "castet-xml-event.h"
#include "castet-xml-event-target.h"
#include "castet-dom-private.h"
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

static GQuark quark_object_actor;

struct _CastetBuildTraverserPrivate
{
    ClutterContainer *container;
    ClutterColor *fill_color;
    ClutterScore *score;

    CastetJsContext *js_context;
};

static void
xml_event_from_clutter_event (CastetXmlEvent       *xml_event,
                              ClutterEvent         *clutter_event,
                              CastetXmlEventTarget *target)
{
    switch (clutter_event->type) {
    case CLUTTER_BUTTON_RELEASE:
    {
        CastetXmlMouseEvent *event = &xml_event->mouse_event;

        event->type = CASTET_XML_EVENT_TYPE_CLICK;
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

static ClutterPath *
build_circle_path (CastetCircleElement *circle)
{
    ClutterPath *path;
    ClutterUnits *cx_u, *cy_u, *r_u;
    gfloat cx, cy, r;
    static gfloat k = 4 * (G_SQRT2 - 1) / 3;

    /* Build the ClutterPath */
    cx_u = castet_circle_element_get_cx (circle);
    cy_u = castet_circle_element_get_cy (circle);
    r_u  = castet_circle_element_get_r (circle);
    cx = clutter_units_to_pixels (cx_u);
    cy = clutter_units_to_pixels (cy_u);
    r = clutter_units_to_pixels (r_u);

    path = clutter_path_new ();

    clutter_path_add_move_to (path, cx + r, cy);
    clutter_path_add_curve_to (path,
                               cx + r, cy + r * k,
                               cx + r * k, cy + r,
                               cx, cy + r);
    clutter_path_add_curve_to (path,
                               cx - r * k , cy + r,
                               cx - r , cy + r * k,
                               cx - r, cy);
    clutter_path_add_curve_to (path,
                               cx - r, cy - r * k,
                               cx - r * k, cy - r,
                               cx, cy - r);
    clutter_path_add_curve_to (path,
                               cx + r * k, cy - r,
                               cx + r, cy - r * k,
                               cx + r, cy);
    clutter_path_add_close (path);

    return path;
}

static void
on_circle_changed (CastetCircleElement *circle,
                   GParamSpec          *pspec,
                   ClutterActor        *target)
{
    ClutterPath *path;

    path = build_circle_path (circle);
    g_object_set (target, "path", path, NULL);
}

static void
castet_build_traverser_traverse_circle (CastetTraverser     *traverser,
                                        CastetCircleElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    CastetElement *element = CASTET_ELEMENT (node);
    const ClutterColor *fill_color, *stroke_color;
    ClutterActor *circle;
    ClutterPath *path;

    path = build_circle_path (node);

    circle = clutter_shape_new ();
    g_object_set (circle, "path", path, NULL);

    /* handle fill / stroke colors */
    fill_color = castet_element_get_fill_color (element);
    stroke_color = castet_element_get_stroke_color (element);
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
static void
castet_build_traverser_traverse_script (CastetTraverser     *traverser,
                                        CastetScriptElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    GError *error = NULL;
    const gchar *script;
    int retval;

    script = castet_script_element_get_code (node);

    if (!castet_js_context_eval (priv->js_context,
                                 script,
                                 strlen (script),
                                 "svg",
                                 &retval,
                                 &error))
    {
        g_message ("Error: %s", error->message);
    }
}

static gboolean
on_button_release_event (ClutterActor *actor,
                         ClutterEvent *event,
                         gpointer      user_data)
{
    CastetXmlEvent xml_event;
    CastetDomElement *target_element = CASTET_DOM_ELEMENT (user_data);
    CastetXmlEventTarget *target = CASTET_XML_EVENT_TARGET (user_data);

    xml_event_from_clutter_event (&xml_event, event, target);

    _castet_dom_element_handle_event (target_element,
                                      castet_xml_event_copy (&xml_event));
    return TRUE;
}

static void
castet_build_traverser_traverse_handler (CastetTraverser      *traverser,
                                         CastetHandlerElement *node)
{
#if 0
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
#endif
    CastetDomElement *target;
    ClutterActor *target_actor;
    CastetXmlEventTarget *event_target;
    CastetXmlEventListener *event_listener;
    CastetXmlEventType event_type;
    const gchar *event_name;

    target = castet_handler_element_get_target (node);

    g_object_get (node, "event", &event_type, NULL);
    event_name = castet_enum_to_string (CASTET_TYPE_XML_EVENT_TYPE, event_type);

    target_actor = g_object_get_qdata (G_OBJECT (target), quark_object_actor);
    if (target_actor == NULL) {
        g_warning (G_STRLOC ": Target has no ClutterActor bound to it");
        return;
    }
    clutter_actor_set_reactive (target_actor, TRUE);
    switch (event_type) {
    case CASTET_XML_EVENT_TYPE_CLICK:
        g_signal_connect (target_actor, "button-release-event",
                          G_CALLBACK (on_button_release_event), target);
        break;
    case CASTET_XML_EVENT_TYPE_NONE:
    default:
        g_warning (G_STRLOC ": Unkown event %d", event_type);
    }

    event_target = CASTET_XML_EVENT_TARGET (target);
    event_listener = CASTET_XML_EVENT_LISTENER (node);
    castet_xml_event_target_add_event_listener (event_target,
                                                event_name,
                                                event_listener,
                                                FALSE);

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

    quark_object_actor = g_quark_from_static_string ("clutter-actor");

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
    traverser_class->traverse_circle = castet_build_traverser_traverse_circle;
    traverser_class->traverse_script = castet_build_traverser_traverse_script;
    traverser_class->traverse_handler = castet_build_traverser_traverse_handler;
}

static void
castet_build_traverser_init (CastetBuildTraverser *self)
{
    CastetBuildTraverserPrivate *priv;

    self->priv = priv = BUILD_TRAVERSER_PRIVATE (self);

    priv->score = clutter_score_new ();
    priv->js_context = castet_js_context_get_default ();
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
