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

#include "dax-dom.h"

#include "dax-internals.h"
#include "dax-debug.h"

#include "dax-udom-svg-timer.h"

static void dax_xml_event_target_init (DaxXmlEventTargetIface *iface);

G_DEFINE_TYPE_WITH_CODE (DaxSvgTimer,
                         dax_svg_timer,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (DAX_TYPE_XML_EVENT_TARGET,
                                                dax_xml_event_target_init))

#define SVG_TIMER_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_SVG_TIMER,   \
                                      DaxSvgTimerPrivate))

enum
{
    PROP_0,

    PROP_DELAY,
    PROP_REPEAT_INTERVAL,
    PROP_RUNNING
};

struct _DaxSvgTimerPrivate
{
    glong repeat_interval;
    gboolean running;

    glong delay;                /* delay used when the timeout was added */
    GTimer *elapsed;            /* time elapsed since the timeout was added */
    guint timeout_id;
};

static gulong
elapsed_ms (GTimer *timer)
{
    gdouble seconds;

    seconds = g_timer_elapsed (timer, NULL);

    return (gulong)(seconds * 1e3);
}

/* priv->delay is the delay till the next wake up */
static gboolean dax_svg_timer_fire (gpointer data);
static void
schedule_next_wake_up (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv = timer->priv;

    DAX_NOTE (SCRIPT, "[TIMER] next wake up in %ld ms", priv->delay);
    g_timer_start (priv->elapsed);
    priv->timeout_id = g_timeout_add (priv->delay,
                                      dax_svg_timer_fire,
                                      timer);
}

static gboolean
dax_svg_timer_fire (gpointer data)
{
    DaxSvgTimer *timer = (DaxSvgTimer *) data;
    DaxSvgTimerPrivate *priv = timer->priv;
    DaxXmlEventTarget *target = DAX_XML_EVENT_TARGET (timer);
    DaxXmlEvent timer_event;

    dax_xml_event_from_type (&timer_event,
                             DAX_XML_EVENT_TYPE_SVG_TIMER,
                             target);
    dax_xml_event_target_handle_event (target,
                                       dax_xml_event_copy (&timer_event));

    if (priv->repeat_interval > 0) {
        priv->delay = priv->repeat_interval;
        schedule_next_wake_up (timer);
    }

    return FALSE;
}

static void
dax_svg_timer_set_delay (DaxSvgTimer *timer,
                         glong        delay)
{
    DaxSvgTimerPrivate *priv = timer->priv;

    DAX_NOTE (SCRIPT, "[TIMER] delay is now %ld ms", delay);

    if (!priv->running) {
        /* initial delay that will be used by start() */
        priv->delay = delay;
    } else if (delay < 0) {
        /* Assigning a negative value is equivalent to calling stop() */
        priv->delay = delay;
        dax_svg_timer_stop (timer);
    } else if (delay == 0) {
        /* 0 means that the event will be triggered as soon as possible */
        dax_svg_timer_fire (timer);
    } else {
        /* we need to delete the next timer event (if needed)... */
        if (priv->timeout_id) {
            g_source_remove (priv->timeout_id);
            priv->timeout_id = 0;
        }

        /* ...and schedule the next one */
        priv->delay = delay;
        schedule_next_wake_up (timer);
    }
}

static glong
dax_svg_timer_get_delay (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv = timer->priv;
    glong remaining;

    remaining = priv->delay - elapsed_ms (priv->elapsed);

    if (remaining < 0)
        remaining = 0;

    DAX_NOTE (SCRIPT, "[TIMER] delay is %ld ms", remaining);

    return remaining;
}

static void
dax_svg_timer_set_repeat_interval (DaxSvgTimer *timer,
                                   glong        repeat_interval)
{
    DAX_NOTE (SCRIPT, "[TIMER] repeat interval is now %ld ms",
              repeat_interval);

    timer->priv->repeat_interval = repeat_interval;
}

static glong
dax_svg_timer_get_repeat_interval (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv = timer->priv;

    DAX_NOTE (SCRIPT, "[TIMER] repeat interval is now %ld",
              priv->repeat_interval);

    return priv->repeat_interval;
}

/*
 * DaxXmlEventTarget "implementation"
 */

static void
dax_xml_event_target_init (DaxXmlEventTargetIface *iface)
{
    /* we use the default implementation of XmlEventTarget */
}

/*
 * GObject implementation
 */

static void
dax_svg_timer_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    DaxSvgTimer *timer = (DaxSvgTimer *) object;

    switch (property_id)
    {
    case PROP_DELAY:
        g_value_set_long (value, dax_svg_timer_get_delay (timer));
        break;
    case PROP_REPEAT_INTERVAL:
        g_value_set_long (value, dax_svg_timer_get_repeat_interval (timer));
        break;
    case PROP_RUNNING:
        g_value_set_boolean (value, timer->priv->running);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_svg_timer_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    DaxSvgTimer *timer = (DaxSvgTimer *) object;

    switch (property_id)
    {
    case PROP_DELAY:
        dax_svg_timer_set_delay (timer, g_value_get_long (value));
        break;
    case PROP_REPEAT_INTERVAL:
        dax_svg_timer_set_repeat_interval (timer, g_value_get_long (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_svg_timer_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_svg_timer_parent_class)->finalize (object);
}

static void
dax_svg_timer_class_init (DaxSvgTimerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxSvgTimerPrivate));

    object_class->get_property = dax_svg_timer_get_property;
    object_class->set_property = dax_svg_timer_set_property;
    object_class->finalize = dax_svg_timer_finalize;

    pspec = g_param_spec_long ("delay",
                               "delay",
                               "time remaining (in ms) until the next event "
                               "is fired",
                               G_MINLONG, G_MAXLONG, 0,
                               DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_DELAY, pspec);

    pspec = g_param_spec_long ("repeatInterval",
                               "repeatInterval",
                               "The interval (in ms) for each repeat of the "
                               "timer",
                               G_MINLONG, G_MAXLONG, 0,
                               DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_REPEAT_INTERVAL, pspec);

    pspec = g_param_spec_boolean ("running",
                                  "running",
                                  "Timer running state",
                                  FALSE,
                                  DAX_GPARAM_READABLE);
    g_object_class_install_property (object_class, PROP_RUNNING, pspec);
}

static void
dax_svg_timer_init (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv;

    timer->priv = priv = SVG_TIMER_PRIVATE (timer);

    priv->elapsed = g_timer_new ();
}

DaxSvgTimer *
dax_svg_timer_new (glong initial_interval,
                   glong repeat_interval)
{
    return g_object_new (DAX_TYPE_SVG_TIMER,
                         "delay", initial_interval,
                         "repeatInterval", repeat_interval,
                         NULL);
}

void
dax_svg_timer_start (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv;

    g_return_if_fail (DAX_IS_SVG_TIMER (timer));

    DAX_NOTE (SCRIPT, "[TIMER] start on %p ms", timer);

    priv = timer->priv;

    if (priv->running)
        return;

    if (priv->delay < 0)
        priv->delay = priv->repeat_interval;

    priv->running = TRUE;
    schedule_next_wake_up (timer);
}

void
dax_svg_timer_stop (DaxSvgTimer *timer)
{
    DaxSvgTimerPrivate *priv;

    g_return_if_fail (DAX_IS_SVG_TIMER (timer));

    DAX_NOTE (SCRIPT, "[TIMER] stop on %p ms", timer);

    priv = timer->priv;

    if (priv->running)
        return;

    priv->running = FALSE;
    g_timer_stop (priv->elapsed);
    g_source_remove (priv->timeout_id);
    priv->timeout_id = 0;
}
