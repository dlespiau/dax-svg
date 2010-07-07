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

#include <stdlib.h>

#include <glib.h>
#include <glib/gprintf.h>
#include <clutter/clutter.h>
#include <dax.h>

#include "pp-super-aa.h"

int
main (int   argc,
      char *argv[])
{
    ClutterActor *stage, *svg, *aa;

    dax_init (&argc, &argv);
    clutter_init (&argc, &argv);

    if (argc < 2) {
        g_printf ("Usage: dax-viewer filename\n");
        return EXIT_FAILURE;
    }

    svg = dax_actor_new_from_file (argv[1], NULL);
    if (svg == NULL) {
        g_printf ("Could not create the SVG actor: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    stage = clutter_stage_get_default ();
    aa = pp_super_aa_new ();
    pp_super_aa_set_resolution (PP_SUPER_AA (aa), 2, 2);
    clutter_container_add_actor (CLUTTER_CONTAINER (aa), svg);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), aa);
    clutter_actor_show_all (stage);
#if 0
    clutter_actor_animate (svg, CLUTTER_LINEAR, 5000, "x", 1000.0f, NULL);
#endif

    dax_actor_play (DAX_ACTOR (svg));

    clutter_main();

    return EXIT_SUCCESS;
}
