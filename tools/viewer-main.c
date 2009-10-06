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

#include <stdlib.h>

#include <glib.h>
#include <glib/gprintf.h>
#include <clutter/clutter.h>
#include <castet.h>

int
main (int   argc,
      char *argv[])
{
    ClutterActor *stage, *svg;

    castet_init (&argc, &argv);
    clutter_init (&argc, &argv);

    if (argc < 2) {
        g_printf ("Usage: castet-viewer filename\n");
        return EXIT_FAILURE;
    }

    svg = castet_actor_new_from_file (argv[1], NULL);
    if (svg == NULL) {
        g_printf ("Could not create the SVG actor: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    stage = clutter_stage_get_default ();
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), svg);
    clutter_actor_show_all (stage);
#if 0
    clutter_actor_animate (svg, CLUTTER_LINEAR, 5000, "x", 1000.0f, NULL);
#endif

    clutter_main();

    return EXIT_SUCCESS;
}
