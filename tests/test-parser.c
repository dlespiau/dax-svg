
#include <glib.h>

#include <castet.h>

static void
test_simple_document_from_file (void)
{
    CastetDomDocument *document, *temp_doc;
    CastetDomNode *svg, *desc, *rect, *temp;
    ClutterUnits *units;

    document = castet_dom_document_new_from_file ("01_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));
    temp = castet_dom_node_get_parent_node (svg);
    g_assert (temp == CASTET_DOM_NODE (document));
    temp = castet_dom_node_get_previous_sibling (svg);
    g_assert (temp == NULL);
    temp = castet_dom_node_get_next_sibling (svg);
    g_assert (temp == NULL);

#if 0
    temp_doc = castet_dom_node_get_owner_document (svg);
    g_assert (temp_doc == document);
#endif

    /* <desc> */
    desc = castet_dom_node_get_first_child (svg);
    g_assert (desc);
    g_assert (CASTET_IS_DESC_ELEMENT (desc));
    temp = castet_dom_node_get_parent_node (desc);
    g_assert (temp == svg);

    /* <rect> */
    rect = castet_dom_node_get_last_child (svg);
    g_assert (rect);
    g_assert (CASTET_IS_RECT_ELEMENT (rect));
    temp = castet_dom_node_get_parent_node (rect);
    g_assert (temp == svg);
    temp = castet_dom_node_get_previous_sibling (rect);
    g_assert (temp == desc);

    temp = castet_dom_node_get_next_sibling (desc);
    g_assert (temp == rect);

    g_object_get (G_OBJECT (rect), "x", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 10.0f);
    g_object_get (G_OBJECT (rect), "y", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 10.0f);
    g_object_get (G_OBJECT (rect), "width", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 10.0f);
    g_object_get (G_OBJECT (rect), "height", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 10.0f);

}

static void
test_polyline (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *polyline;
    CastetKnotSequence *seq;
    const gfloat *array;

    document = castet_dom_document_new_from_file ("09_06.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    polyline = castet_dom_node_get_last_child (svg);
    g_assert (polyline);
    g_assert (CASTET_IS_POLYLINE_ELEMENT (polyline));

    g_object_get (G_OBJECT (polyline), "points", &seq, NULL);
    g_assert (castet_knot_sequence_get_size (seq) == 22);
    array = castet_knot_sequence_get_array (seq);
    g_assert_cmpfloat (array[0], ==, 50.0f);
    g_assert_cmpfloat (array[1], ==, 375.0f);
    g_assert_cmpfloat (array[13], ==, 250.0f);
    g_assert_cmpfloat (array[14], ==, 450.0f);
    g_assert_cmpfloat (array[42], ==, 1150.0f);
    g_assert_cmpfloat (array[43], ==, 375.0f);
}


int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    castet_init (&argc, &argv);

    g_test_add_func ("/parser/simple-document-from-file",
                     test_simple_document_from_file);
    g_test_add_func ("/parser/polyline", test_polyline);

    return g_test_run ();
}
