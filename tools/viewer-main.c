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
#include <clutter/x11/clutter-x11.h>
#include <dax.h>
#include <X11/Xlib.h>

#include "pp-super-aa.h"

static gboolean nugget = FALSE;

static GOptionEntry entries[] =
{
    { "nugget", 'a', 0, G_OPTION_ARG_NONE, &nugget,
      "Use a transparent stage", NULL },
    { NULL }
};

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long inputMode;
    unsigned long status;
} PropMotifWmHints;

static void
hide_decorations (ClutterStage *stage)
{
    Display *dpy;
    Window win;

    static Atom motif_wm_hints_atom = None;

    dpy = clutter_x11_get_default_display ();
    win = clutter_x11_get_stage_window (stage);

    if (win == None)
        return;

    if (!motif_wm_hints_atom)
        motif_wm_hints_atom = XInternAtom (dpy, "_MOTIF_WM_HINTS", False);

    /* Remove/add the window decorations */
    if (motif_wm_hints_atom) {
        PropMotifWmHints new_hints = {0,};
        PropMotifWmHints *hints;

        hints = &new_hints;

        hints->flags = 0x2;
        hints->functions = 0x0;
        hints->decorations = 0x0;

        XChangeProperty (dpy, win, motif_wm_hints_atom, motif_wm_hints_atom,
                         32, PropModeReplace, (guchar*) hints,
                         sizeof(PropMotifWmHints)/ sizeof (long));
    }

}

int
main (int   argc,
      char *argv[])
{
    ClutterActor *stage, *svg, *aa;
    GOptionContext *context;
    GError *error = NULL;

    clutter_x11_set_use_argb_visual (TRUE);

    context = g_option_context_new ("- View SVG documents");
    g_option_context_add_main_entries (context, entries, NULL);
    g_option_context_add_group (context, cogl_get_option_group ());
    g_option_context_add_group (context, clutter_get_option_group ());
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        return EXIT_FAILURE;
    }

    dax_init (&argc, &argv);
    clutter_init (&argc, &argv);

    if (argc < 2) {
        g_printf ("Usage: dax-viewer filename\n");
        return EXIT_FAILURE;
    }

    stage = clutter_stage_get_default ();
    if (nugget) {
        clutter_stage_set_use_alpha (CLUTTER_STAGE (stage), TRUE);
        clutter_actor_set_opacity (stage, 0);
        hide_decorations (CLUTTER_STAGE (stage));
    }


    svg = dax_actor_new_from_file (argv[1], NULL);
    if (svg == NULL) {
        g_printf ("Could not create the SVG actor: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    aa = pp_super_aa_new ();
    pp_super_aa_set_resolution (PP_SUPER_AA (aa), 2, 2);
    clutter_container_add_actor (CLUTTER_CONTAINER (aa), svg);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), aa);
    clutter_actor_show_all (stage);

    dax_actor_play (DAX_ACTOR (svg));

    clutter_main();

    return EXIT_SUCCESS;
}
