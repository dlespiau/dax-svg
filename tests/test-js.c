/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <glib/gstdio.h>
#include <string.h>

#include <dax.h>

typedef struct {
    DaxDomDocument *document;
} DaxTestJSFixture;

const gchar abs_top_srcdir[] = DAX_ABS_TOP_SRCDIR;

static void
setup(DaxTestJSFixture *fix,
      gconstpointer     test_data)
{
}

static void
teardown(DaxTestJSFixture *fix,
         gconstpointer     test_data)
{
    g_object_unref(fix->document);
}

static void
test(DaxTestJSFixture *fix,
     gconstpointer     test_data)
{
    const gchar *filename = test_data;
    DaxTraverser *load_traverser;
    DaxJsContext *context;
    GError *error = NULL;
    gint retval;

    fix->document = dax_dom_document_new_from_file (filename, &error);
    if (fix->document == NULL) {
        g_error("%s", error->message);
        g_clear_error (&error);
    }
    g_assert(error == NULL);

    /* Apply the load traverser, a traverser that will execute "load" handler
     * elements */
    load_traverser = dax_traverser_load_new (DAX_DOM_NODE (fix->document));
    dax_traverser_apply (load_traverser);
    g_object_unref (load_traverser);

    context = dax_dom_document_get_js_context (fix->document);
    dax_js_context_eval (context, "daxSuccess", -1, "test-js", &retval, &error);
    if (error != NULL) {
        g_error ("%s", error->message);
        g_clear_error (&error);
    }
    g_assert_cmpint (retval, ==, TRUE);
}

gint
main(gint    argc,
     gchar **argv)
{
    const gchar *js_test_dir;
    const gchar *name;
    GDir *dir;

    g_test_init(&argc, &argv, NULL);
    g_type_init();
    dax_init (&argc, &argv);

    /* iterate through all 'test*.js' files in ${top_srcdir}/test/js */
    js_test_dir = g_build_filename(abs_top_srcdir, "tests", "js", NULL);
    dir = g_dir_open(js_test_dir, 0, NULL);
    g_assert(dir != NULL);

    while ((name = g_dir_read_name(dir)) != NULL) {
        char *test_name;
        char *file_name;

        if (!(g_str_has_prefix(name, "test") &&
              g_str_has_suffix(name, ".svg")))
            continue;

        /* pretty print, drop 'test' prefix and '.js' suffix from test name */
        test_name = g_strconcat("/js/", name + 4, NULL);
        test_name[strlen(test_name)-4] = '\0';

        file_name = g_build_filename(js_test_dir, name, NULL);
        g_test_add(test_name, DaxTestJSFixture, file_name,
                   setup, test, teardown);
        g_free(test_name);
        /* not freeing file_name as it's needed while running the test */
    }
    g_dir_close(dir);

    return g_test_run ();
}
