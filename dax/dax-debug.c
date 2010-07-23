/*
 * Dax - Load and draw SVG
 *
 * Copyright (C) 2009, 2010 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License, version 2.1, as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>..
 */

/*
 * Originally taken from clutter-gst, relicensed to LGPL 2.1 as allowed by
 * the original material license (the original code is under LGPL 2+).
 *      Copyright (C) 2006-2008 OpenedHand
 *      Copyright (C) 2009 Intel Corporation
 */

#include <glib.h>

#include "dax-debug.h"

#ifdef DAX_ENABLE_DEBUG

guint _dax_debug_flags = 0;  /* global debug flag */

static GTimer *dax_timer;

static const GDebugKey dax_debug_keys[] = {
  { "misc",      DAX_DEBUG_MISC      },
  { "parsing",   DAX_DEBUG_PARSING   },
  { "namespace", DAX_DEBUG_NAMESPACE },
  { "traverser", DAX_DEBUG_TRAVERSER },
  { "event",     DAX_DEBUG_EVENT     },
  { "loading",   DAX_DEBUG_LOADING   },
  { "script",    DAX_DEBUG_SCRIPT    },
  { "animation", DAX_DEBUG_ANIMATION },
  { "transform", DAX_DEBUG_TRANSFORM }
};

/**
 * dax_get_timestamp:
 *
 * Returns the approximate number of microseconds passed since dax was
 * intialized.
 *
 * Return value: Number of microseconds since dax_init() was called.
 */
gulong
_dax_get_timestamp (void)
{
  gdouble seconds;

  seconds = g_timer_elapsed (dax_timer, NULL);

  return (gulong)(seconds / 1.0e-6);
}

gboolean
_dax_debug_init (void)
{
  const char *env_string;

  env_string = g_getenv ("DAX_DEBUG");

  dax_timer = g_timer_new ();
  g_timer_start (dax_timer);

  if (env_string == NULL)
    return TRUE;

  _dax_debug_flags = g_parse_debug_string (env_string,
                                           dax_debug_keys,
                                           G_N_ELEMENTS (dax_debug_keys));

  return TRUE;
}

#endif /* DAX_ENABLE_DEBUG */
