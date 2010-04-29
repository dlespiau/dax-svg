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

#ifndef __DAX_SVG_TIMER_H__
#define __DAX_SVG_TIMER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define DAX_TYPE_SVG_TIMER dax_svg_timer_get_type()

#define DAX_SVG_TIMER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_SVG_TIMER, DaxSvgTimer))

#define DAX_SVG_TIMER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_SVG_TIMER, DaxSvgTimerClass))

#define DAX_IS_SVG_TIMER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_SVG_TIMER))

#define DAX_IS_SVG_TIMER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_SVG_TIMER))

#define DAX_SVG_TIMER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_SVG_TIMER, DaxSvgTimerClass))

typedef struct _DaxSvgTimer DaxSvgTimer;
typedef struct _DaxSvgTimerClass DaxSvgTimerClass;
typedef struct _DaxSvgTimerPrivate DaxSvgTimerPrivate;

struct _DaxSvgTimer
{
    GObject parent;

    DaxSvgTimerPrivate *priv;
};

struct _DaxSvgTimerClass
{
    GObjectClass parent_class;
};

GType           dax_svg_timer_get_type  (void) G_GNUC_CONST;

DaxSvgTimer *   dax_svg_timer_new       (glong initial_interval,
                                         glong repeat_interval);
void            dax_svg_timer_start     (DaxSvgTimer *timer);
void            dax_svg_timer_stop      (DaxSvgTimer *timer);

G_END_DECLS

#endif /* __DAX_SVG_TIMER_H__ */
