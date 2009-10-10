
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

static void
test_duration (void)
{
    CastetDuration duration;

    castet_duration_from_s (&duration, 1.5f);
    g_assert (castet_duration_get_unit_type (&duration) == CASTET_DURATION_S);
    g_assert_cmpfloat (castet_duration_get_unit_value (&duration), ==, 1.5f);
    g_assert_cmpfloat (castet_duration_to_ms (&duration), ==, 1500.f);

    castet_duration_from_ms (&duration, 200.f);
    g_assert (castet_duration_get_unit_type (&duration) == CASTET_DURATION_MS);
    g_assert_cmpfloat (castet_duration_get_unit_value (&duration), ==, 200.f);
    g_assert_cmpfloat (castet_duration_to_ms (&duration), ==, 200.f);
}

static void
test_duration_string (void)
{
    CastetDuration duration;

    g_assert (castet_duration_from_string (&duration, " ") == FALSE);

    g_assert (castet_duration_from_string (&duration, " 15.2 s   ") == TRUE);
    g_assert (castet_duration_get_unit_type (&duration) == CASTET_DURATION_S);
    g_assert_cmpfloat (castet_duration_get_unit_value (&duration), ==, 15.2f);

    g_assert (castet_duration_from_string (&duration, "150ms") == TRUE);
    g_assert (castet_duration_get_unit_type (&duration) == CASTET_DURATION_MS);
    g_assert_cmpfloat (castet_duration_get_unit_value (&duration), ==, 150.f);

    g_assert (castet_duration_from_string (&duration, "1.5") == TRUE);
    g_assert (castet_duration_get_unit_type (&duration) == CASTET_DURATION_S);
    g_assert_cmpfloat (castet_duration_get_unit_value (&duration), ==, 1.5f);

    g_assert (castet_duration_from_string (&duration, " 150 stupid") == FALSE);

    castet_duration_from_s (&duration, 1.5f);
    g_assert_cmpstr (castet_duration_to_string (&duration), ==, "1.50s");

    castet_duration_from_ms (&duration, 200.f);
    g_assert_cmpstr (castet_duration_to_string (&duration), ==, "200.00ms");

}

static void
test_repeat_count (void)
{
    CastetRepeatCount repeat_count;

    castet_repeat_count_from_number (&repeat_count, 3);
    g_assert_cmpfloat (castet_repeat_count_get_value (&repeat_count), ==, 3);
    g_assert (castet_repeat_count_is_indefinite (&repeat_count) == FALSE);
}

static void
test_repeat_count_string (void)
{
    CastetRepeatCount count;

    g_assert (castet_repeat_count_from_string (&count, "  ") == FALSE);

    g_assert (castet_repeat_count_from_string (&count, " 3.2    ") == TRUE);
    g_assert_cmpfloat (castet_repeat_count_get_value (&count), ==, 3.2f);

    g_assert (castet_repeat_count_from_string (&count, " indefinite    "));
    g_assert (castet_repeat_count_is_indefinite (&count));
    g_assert_cmpfloat (castet_repeat_count_get_value (&count), ==, 0.f);
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
    g_test_add_func ("/types/duration", test_duration);
    g_test_add_func ("/types/duration-string", test_duration_string);
    g_test_add_func ("/types/repeat-count", test_repeat_count);
    g_test_add_func ("/types/repeat-count-string", test_repeat_count_string);

    return g_test_run ();
}
