
#include <glib.h>
#include <glib-object.h>

#include <dax-utils.h>

typedef struct _CountTest {
    const char *str;
    guint expected_nr;
} CountTest;

static void
test_utils_count (void)
{
    guint i;
    CountTest count_words[10] = {
        { "", 0 },
        { " \t", 0 },
        { "one", 1 },
        { " one", 1 },
        { "one ", 1 },
        { " one ", 1 },
        { "two words", 2 },
        { " two words", 2 },
        { "two words ", 2 },
        { " two words ", 2 }
    };

    for (i = 0; i < G_N_ELEMENTS (count_words); i++) {
        guint nr;

        nr = _dax_utils_count_words (count_words[i].str);
        g_assert_cmpint (count_words[i].expected_nr, ==, nr);
    }
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);

    g_test_add_func ("/utils/count", test_utils_count);

    return g_test_run ();
}
