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

typedef struct {
    ClutterActor *stage, *svg;
    ClutterActor *context_menu;
    gulong captured_event_handler;
    MxAction *pause_action, *play_action, *quit_action;
} DaxViewer;

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

static gboolean
on_stage_captured_event (ClutterActor *actor,
                         ClutterEvent *event,
                         DaxViewer    *viewer)
{
    if (event->type == CLUTTER_BUTTON_PRESS &&
        clutter_event_get_button (event) == 3)
    {
        ClutterButtonEvent *bevent = (ClutterButtonEvent *) event;

        mx_menu_show_with_position (MX_MENU (viewer->context_menu),
                                    bevent->x, bevent->y);
        return TRUE;
    }

    return FALSE;
}

static void
on_quit_clicked (MxAction  *action,
                 DaxViewer *viewer)
{
    clutter_main_quit ();
}

static void
on_play_clicked (MxAction  *action,
                 DaxViewer *viewer)
{
    dax_actor_set_playing (DAX_ACTOR (viewer->svg), TRUE);
    clutter_actor_hide (viewer->context_menu);
    mx_menu_remove_all (MX_MENU (viewer->context_menu));
    mx_menu_add_action (MX_MENU (viewer->context_menu), viewer->pause_action);
    mx_menu_add_action (MX_MENU (viewer->context_menu), viewer->quit_action);
}

static void
on_pause_clicked (MxAction  *action,
                  DaxViewer *viewer)
{
    dax_actor_set_playing (DAX_ACTOR (viewer->svg), FALSE);
    clutter_actor_hide (viewer->context_menu);
    mx_menu_remove_all (MX_MENU (viewer->context_menu));
    mx_menu_add_action (MX_MENU (viewer->context_menu), viewer->play_action);
    mx_menu_add_action (MX_MENU (viewer->context_menu), viewer->quit_action);
}

int
main (int   argc,
      char *argv[])
{
    ClutterActor *stage, *aa, *overlay;
    GOptionContext *context;
    GError *error = NULL;
    DaxViewer viewer;
    MxAction *action;

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

    /* Setup the stage */
    stage = clutter_stage_get_default ();
    viewer.stage = stage;
    if (nugget) {
        clutter_stage_set_use_alpha (CLUTTER_STAGE (stage), TRUE);
        clutter_actor_set_opacity (stage, 0);
        hide_decorations (CLUTTER_STAGE (stage));
    }

    viewer.captured_event_handler =
        g_signal_connect (stage, "captured-event",
                          G_CALLBACK (on_stage_captured_event), &viewer);

    /* Create the context menu */
    viewer.context_menu = mx_menu_new();
    action = mx_action_new_full ("pause", "Pause",
                                 G_CALLBACK (on_pause_clicked), &viewer);
    mx_menu_add_action (MX_MENU (viewer.context_menu), action);
    viewer.pause_action = g_object_ref (action);
    action = mx_action_new_full ("quit", "Quit",
                                 G_CALLBACK (on_quit_clicked), &viewer);
    mx_menu_add_action (MX_MENU (viewer.context_menu), action);
    viewer.quit_action = g_object_ref (action);
    action = mx_action_new_full ("play", "Play",
                                 G_CALLBACK (on_play_clicked), &viewer);
    viewer.play_action = g_object_ref (action);

    /* Create the SVG actor */
    viewer.svg = dax_actor_new_from_file (argv[1], NULL);
    if (viewer.svg == NULL) {
        g_printf ("Could not create the SVG actor: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Pack the DaxActor into a PPSuperAA for 2x2 multisampling */
    aa = pp_super_aa_new ();
    pp_super_aa_set_resolution (PP_SUPER_AA (aa), 2, 2);
    mx_offscreen_set_pick_child (MX_OFFSCREEN (aa), TRUE);
    clutter_container_add_actor (CLUTTER_CONTAINER (aa), viewer.svg);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), aa);

    /* Pack the context menu into an overlay group */
    overlay = clutter_group_new ();
    clutter_container_add_actor (CLUTTER_CONTAINER (overlay),
                                 viewer.context_menu);
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), overlay);

    clutter_actor_show_all (stage);

    dax_actor_set_playing (DAX_ACTOR (viewer.svg), TRUE);

    clutter_main();

    g_signal_handler_disconnect (stage, viewer.captured_event_handler);
    g_object_unref (viewer.play_action);
    g_object_unref (viewer.pause_action);

    return EXIT_SUCCESS;
}
