
#include <glib.h>

#include <castet.h>

static void
test_simple_document_from_file (void)
{
    CastetDomDocument *document, *temp_doc;
    CastetDomNode *svg, *desc, *rect, *temp;
    ClutterUnits *units;
    CastetSvgVersion version;
    CastetSvgBaseProfile profile;

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
    g_object_get (svg, "version", &version, NULL);
    g_assert_cmpint (version, ==, CASTET_SVG_VERSION_1_2);
    g_object_get (svg, "baseProfile", &profile, NULL);
    g_assert_cmpint (profile, ==, CASTET_SVG_BASE_PROFILE_TINY);

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
    ClutterUnits *units;

    document = castet_dom_document_new_from_file ("09_06.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));
    /* let's test <svg> width and height attribute here as 09_06.svg had them
     * with height != width */
    g_object_get (G_OBJECT (svg), "width", &units, NULL);
    g_assert_cmpint (clutter_units_get_unit_type (units), ==, CLUTTER_UNIT_CM);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 12.0f);
    g_object_get (G_OBJECT (svg), "height", &units, NULL);
    g_assert_cmpint (clutter_units_get_unit_type (units), ==, CLUTTER_UNIT_CM);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 4.0f);


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

static void
test_path (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *path;
    ClutterPath *clutter_path;
    ClutterPathNode node;

    document = castet_dom_document_new_from_file ("08_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    path = castet_dom_node_get_last_child (svg);
    g_assert (CASTET_IS_PATH_ELEMENT (path));
    clutter_path = castet_path_element_get_path (CASTET_PATH_ELEMENT (path));
    g_assert (clutter_path_get_n_nodes (clutter_path) == 4);
    clutter_path_get_node (clutter_path, 0, &node);
    g_assert (node.type == CLUTTER_PATH_MOVE_TO);
    g_assert_cmpint (node.points[0].x, ==, 100);
    g_assert_cmpint (node.points[0].y, ==, 100);
    clutter_path_get_node (clutter_path, 1, &node);
    g_assert (node.type == CLUTTER_PATH_LINE_TO);
    g_assert_cmpint (node.points[0].x, ==, 300);
    g_assert_cmpint (node.points[0].y, ==, 100);
    clutter_path_get_node (clutter_path, 2, &node);
    g_assert (node.type == CLUTTER_PATH_LINE_TO);
    g_assert_cmpint (node.points[0].x, ==, 200);
    g_assert_cmpint (node.points[0].y, ==, 300);
    clutter_path_get_node (clutter_path, 3, &node);
    g_assert (node.type == CLUTTER_PATH_CLOSE);
}

static void
test_animate (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *rect, *animate;
    CastetAnimationAttributeType attribute_type;
    CastetDuration *duration;
    CastetRepeatCount *count;
    gchar *string;

    document = castet_dom_document_new_from_file ("19_011.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    rect = castet_dom_node_get_last_child (svg);
    g_assert (CASTET_IS_RECT_ELEMENT (rect));

    animate = castet_dom_node_get_last_child (rect);
    g_assert (CASTET_IS_ANIMATE_ELEMENT (animate));
    g_object_get (animate, "attributeType", &attribute_type, NULL);
    g_assert_cmpint (attribute_type, ==, CASTET_ANIMATION_ATTRIBUTE_TYPE_CSS);
    g_object_get (animate, "attributeName", &string, NULL);
    g_assert_cmpstr (string, ==, "fill-opacity");
    g_object_get (animate, "from", &string, NULL);
    g_assert_cmpstr (string, ==, "1");
    g_object_get (animate, "to", &string, NULL);
    g_assert_cmpstr (string, ==, "0");
    g_object_get (animate, "dur", &duration, NULL);
    g_assert_cmpfloat (castet_duration_to_ms (duration), ==, 5000.f);
    g_object_get (animate, "repeatCount", &count, NULL);
    g_assert (castet_repeat_count_is_indefinite (count));
}

static void
test_title_desc (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *title, *desc, *text;

    document = castet_dom_document_new_from_file ("08_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    /* <title> */
    title = castet_dom_node_get_first_child (svg);
    g_assert (CASTET_IS_TITLE_ELEMENT (title));

    /* text node of <title> */
    text = castet_dom_node_get_first_child (title);
    g_assert (CASTET_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        castet_dom_character_data_get_data (CASTET_DOM_CHARACTER_DATA (text)),
        ==,
        "Example triangle01- simple example of a 'path'");

    /* <desc> */
    desc = castet_dom_node_get_next_sibling (title);
    g_assert (CASTET_IS_DESC_ELEMENT (desc));

    /* text node of <desc> */
    text = castet_dom_node_get_first_child (desc);
    g_assert (CASTET_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        castet_dom_character_data_get_data (CASTET_DOM_CHARACTER_DATA (text)),
        ==,
        "A path that draws a triangle");
}

static const gchar _18_01_script[] = "\n\
    function circle_click(evt) {\n\
       var circle = evt.target;\n\
       var currentRadius = circle.getFloatTrait(\"r\");\n\
       if (currentRadius == 100)\n\
         circle.setFloatTrait(\"r\", currentRadius*2);\n\
       else\n\
         circle.setFloatTrait(\"r\", currentRadius*0.5);\n\
    }\n\
  ";

static void
test_script (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *desc, *script, *text;
    CastetScriptType type;

    document = castet_dom_document_new_from_file ("18_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    /* <desc> */
    desc = castet_dom_node_get_first_child (svg);
    g_assert (CASTET_IS_DESC_ELEMENT (desc));

    /* <script> */
    script = castet_dom_node_get_next_sibling (desc);
    g_assert (CASTET_IS_SCRIPT_ELEMENT (script));
    g_object_get (script, "type", &type, NULL);
    g_assert_cmpint (type, ==, CASTET_SCRIPT_TYPE_ECMASCRIPT);

    /* text node of <script> */
    text = castet_dom_node_get_first_child (script);
    g_assert (CASTET_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        castet_dom_character_data_get_data (CASTET_DOM_CHARACTER_DATA (text)),
        ==,
        _18_01_script);
}

static void
test_circle (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *circle;
    CastetScriptType type;
    ClutterUnits *units;

    document = castet_dom_document_new_from_file ("09_03.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    /* <circle> */
    circle = castet_dom_node_get_last_child (svg);
    g_assert (CASTET_IS_CIRCLE_ELEMENT (circle));
    g_object_get (circle, "cx", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value (units), ==, 400.0f);
    g_object_get (circle, "cy", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value (units), ==, 200.0f);
    g_object_get (circle, "r", &units, NULL);
    g_assert_cmpfloat (clutter_units_get_unit_value (units), ==, 100.0f);
}

static const gchar _18_01_handler[] = "\n\
         circle_click(evt);\n\
    ";

static void
test_handler (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *text, *circle, *handler, *text_node;
    CastetScriptType type;
    CastetXmlEventType event_type;

    document = castet_dom_document_new_from_file ("18_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    /* <text> */
    text = castet_dom_node_get_last_child (svg);
#if 0
    FIXME enable this when the text element is parsed
    g_assert (CASTET_IS_TEXT_ELEMENT (text));
#endif

    /* <circle> */
    circle = castet_dom_node_get_previous_sibling (text);
    g_assert (CASTET_IS_CIRCLE_ELEMENT (circle));

    /* <handler> */
    handler = castet_dom_node_get_first_child (circle);
    g_assert (CASTET_IS_HANDLER_ELEMENT (handler));
    g_object_get (handler, "type", &type, NULL);
    g_assert_cmpint (type, ==, CASTET_SCRIPT_TYPE_ECMASCRIPT);
    g_object_get (handler, "event", &event_type, NULL);
    g_assert_cmpint (event_type, ==, CASTET_XML_EVENT_TYPE_CLICK);

    /* text node of <handler> */
    text_node = castet_dom_node_get_first_child (handler);
    g_assert (CASTET_IS_DOM_TEXT (text_node));
    g_assert_cmpstr (
        castet_dom_character_data_get_data (CASTET_DOM_CHARACTER_DATA (text_node)),
        ==,
        _18_01_handler);
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
    g_test_add_func ("/parser/path", test_path);
    g_test_add_func ("/parser/animate", test_animate);
    g_test_add_func ("/parser/title-desc", test_title_desc);
    g_test_add_func ("/parser/script", test_script);
    g_test_add_func ("/parser/circle", test_circle);
    g_test_add_func ("/parser/handler", test_handler);

    return g_test_run ();
}
