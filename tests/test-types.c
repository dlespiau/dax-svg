
#include <glib.h>

#include <castet.h>

static void
test_knot_sequence (void)
{
    CastetKnotSequence *seq;
    static float knots[4] = {10.0f, 20.0f, 100.0f, 200.0f};

    seq = castet_knot_sequence_new_from_static_array (knots,
                                                      G_N_ELEMENTS (knots) / 2);
    g_assert (castet_knot_sequence_get_array (seq) == knots);
    g_assert (castet_knot_sequence_get_size (seq) == G_N_ELEMENTS (knots) / 2);
    g_object_unref (seq);
}

static void
test_knot_sequence_string (void)
{
    static gchar string[] = "10,20 100,200";
    CastetKnotSequence *seq;
    const gfloat *array;

    seq = castet_knot_sequence_new_from_string (string);
    g_assert (castet_knot_sequence_get_size (seq) == 2);
    array = castet_knot_sequence_get_array (seq);
    g_assert_cmpfloat (array[0], ==, 10.0f);
    g_assert_cmpfloat (array[1], ==, 20.0f);
    g_assert_cmpfloat (array[2], ==, 100.0f);
    g_assert_cmpfloat (array[3], ==, 200.0f);
    g_object_unref (seq);
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    castet_init (&argc, &argv);

    g_test_add_func ("/types/knot-sequence", test_knot_sequence);
    g_test_add_func ("/types/knot-sequence-string", test_knot_sequence_string);
    return g_test_run ();
}
