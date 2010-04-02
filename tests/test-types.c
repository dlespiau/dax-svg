
#include <glib.h>

#include <dax.h>

static void
test_knot_sequence (void)
{
    DaxKnotSequence *seq;
    static float knots[4] = {10.0f, 20.0f, 100.0f, 200.0f};

    seq = dax_knot_sequence_new_from_static_array (knots,
                                                      G_N_ELEMENTS (knots) / 2);
    g_assert (dax_knot_sequence_get_array (seq) == knots);
    g_assert (dax_knot_sequence_get_size (seq) == G_N_ELEMENTS (knots) / 2);
    g_object_unref (seq);
}

static void
test_knot_sequence_string (void)
{
    static gchar string[] = "10,20 100,200";
    DaxKnotSequence *seq;
    const gfloat *array;

    seq = dax_knot_sequence_new_from_string (string);
    g_assert (dax_knot_sequence_get_size (seq) == 2);
    array = dax_knot_sequence_get_array (seq);
    g_assert_cmpfloat (array[0], ==, 10.0f);
    g_assert_cmpfloat (array[1], ==, 20.0f);
    g_assert_cmpfloat (array[2], ==, 100.0f);
    g_assert_cmpfloat (array[3], ==, 200.0f);
    g_object_unref (seq);
}

static void
test_duration (void)
{
    DaxDuration duration;

    dax_duration_from_s (&duration, 1.5f);
    g_assert (dax_duration_get_unit_type (&duration) == DAX_DURATION_S);
    g_assert_cmpfloat (dax_duration_get_unit_value (&duration), ==, 1.5f);
    g_assert_cmpfloat (dax_duration_to_ms (&duration), ==, 1500.f);

    dax_duration_from_ms (&duration, 200.f);
    g_assert (dax_duration_get_unit_type (&duration) == DAX_DURATION_MS);
    g_assert_cmpfloat (dax_duration_get_unit_value (&duration), ==, 200.f);
    g_assert_cmpfloat (dax_duration_to_ms (&duration), ==, 200.f);
}

static void
test_duration_string (void)
{
    DaxDuration duration;

    g_assert (dax_duration_from_string (&duration, " ") == FALSE);

    g_assert (dax_duration_from_string (&duration, " 15.2 s   ") == TRUE);
    g_assert (dax_duration_get_unit_type (&duration) == DAX_DURATION_S);
    g_assert_cmpfloat (dax_duration_get_unit_value (&duration), ==, 15.2f);

    g_assert (dax_duration_from_string (&duration, "150ms") == TRUE);
    g_assert (dax_duration_get_unit_type (&duration) == DAX_DURATION_MS);
    g_assert_cmpfloat (dax_duration_get_unit_value (&duration), ==, 150.f);

    g_assert (dax_duration_from_string (&duration, "1.5") == TRUE);
    g_assert (dax_duration_get_unit_type (&duration) == DAX_DURATION_S);
    g_assert_cmpfloat (dax_duration_get_unit_value (&duration), ==, 1.5f);

    g_assert (dax_duration_from_string (&duration, " 150 stupid") == FALSE);

    dax_duration_from_s (&duration, 1.5f);
    g_assert_cmpstr (dax_duration_to_string (&duration), ==, "1.50s");

    dax_duration_from_ms (&duration, 200.f);
    g_assert_cmpstr (dax_duration_to_string (&duration), ==, "200.00ms");

}

static void
test_repeat_count (void)
{
    DaxRepeatCount repeat_count;

    dax_repeat_count_from_number (&repeat_count, 3);
    g_assert_cmpfloat (dax_repeat_count_get_value (&repeat_count), ==, 3);
    g_assert (dax_repeat_count_is_indefinite (&repeat_count) == FALSE);
}

static void
test_repeat_count_string (void)
{
    DaxRepeatCount count;

    g_assert (dax_repeat_count_from_string (&count, "  ") == FALSE);

    g_assert (dax_repeat_count_from_string (&count, " 3.2    ") == TRUE);
    g_assert_cmpfloat (dax_repeat_count_get_value (&count), ==, 3.2f);

    g_assert (dax_repeat_count_from_string (&count, " indefinite    "));
    g_assert (dax_repeat_count_is_indefinite (&count));
    g_assert_cmpfloat (dax_repeat_count_get_value (&count), ==, 0.f);
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    dax_init (&argc, &argv);

    g_test_add_func ("/types/knot-sequence", test_knot_sequence);
    g_test_add_func ("/types/knot-sequence-string", test_knot_sequence_string);
    g_test_add_func ("/types/duration", test_duration);
    g_test_add_func ("/types/duration-string", test_duration_string);
    g_test_add_func ("/types/repeat-count", test_repeat_count);
    g_test_add_func ("/types/repeat-count-string", test_repeat_count_string);

    return g_test_run ();
}
