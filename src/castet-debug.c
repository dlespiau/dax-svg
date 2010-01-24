/*
 * Castet - Load and draw SVG
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
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * Originally taken from clutter-gst, relicensed to LGPL 2.1 as allowed by
 * the original material license (the original code is under LGPL 2+).
 *      Copyright (C) 2006-2008 OpenedHand
 *      Copyright (C) 2009 Intel Corporation
 */

#include <glib.h>

#include "castet-debug.h"

#ifdef CASTET_ENABLE_DEBUG

guint _castet_debug_flags = 0;  /* global debug flag */

static GTimer *castet_timer;

static const GDebugKey castet_debug_keys[] = {
  { "misc",      CASTET_DEBUG_MISC      },
  { "parsing",   CASTET_DEBUG_PARSING   },
  { "namespace", CASTET_DEBUG_NAMESPACE },
  { "traverser", CASTET_DEBUG_TRAVERSER },
};

/**
 * castet_get_timestamp:
 *
 * Returns the approximate number of microseconds passed since castet was
 * intialized.
 *
 * Return value: Number of microseconds since castet_init() was called.
 */
gulong
_castet_get_timestamp (void)
{
  gdouble seconds;

  seconds = g_timer_elapsed (castet_timer, NULL);

  return (gulong)(seconds / 1.0e-6);
}

gboolean
_castet_debug_init (void)
{
  const char *env_string;

  env_string = g_getenv ("CASTET_DEBUG");

  castet_timer = g_timer_new ();
  g_timer_start (castet_timer);

  if (env_string == NULL)
    return TRUE;

  _castet_debug_flags =
    g_parse_debug_string (env_string,
                          castet_debug_keys,
                          G_N_ELEMENTS (castet_debug_keys));

  return TRUE;
}

#endif /* CASTET_ENABLE_DEBUG */
