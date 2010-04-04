
#include <glib.h>

#include <dax.h>

static void
test_simple_document_from_file (void)
{
    DaxDomDocument *document, *temp_doc;
    DaxDomNode *svg, *desc, *rect, *temp;
    ClutterUnits *units;
    DaxSvgVersion version;
    DaxSvgBaseProfile profile;
    GArray *view_box;

    document = dax_dom_document_new_from_file ("01_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));
    temp = dax_dom_node_get_parent_node (svg);
    g_assert (temp == DAX_DOM_NODE (document));
    temp = dax_dom_node_get_previous_sibling (svg);
    g_assert (temp == NULL);
    temp = dax_dom_node_get_next_sibling (svg);
    g_assert (temp == NULL);
    g_object_get (svg, "version", &version, NULL);
    g_assert_cmpint (version, ==, DAX_SVG_VERSION_1_2);
    g_object_get (svg, "baseProfile", &profile, NULL);
    g_assert_cmpint (profile, ==, DAX_SVG_BASE_PROFILE_TINY);
    g_object_get (svg, "viewBox", &view_box, NULL);
    g_assert_cmpfloat (g_array_index (view_box, float, 0), ==, 0.0f);
    g_assert_cmpfloat (g_array_index (view_box, float, 1), ==, 0.0f);
    g_assert_cmpfloat (g_array_index (view_box, float, 2), ==, 30.0f);
    g_assert_cmpfloat (g_array_index (view_box, float, 3), ==, 30.0f);

#if 0
    temp_doc = dax_dom_node_get_owner_document (svg);
    g_assert (temp_doc == document);
#endif

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (desc);
    g_assert (DAX_IS_ELEMENT_DESC (desc));
    temp = dax_dom_node_get_parent_node (desc);
    g_assert (temp == svg);

    /* <rect> */
    rect = dax_dom_node_get_last_child (svg);
    g_assert (rect);
    g_assert (DAX_IS_ELEMENT_RECT (rect));
    temp = dax_dom_node_get_parent_node (rect);
    g_assert (temp == svg);
    temp = dax_dom_node_get_previous_sibling (rect);
    g_assert (temp == desc);

    temp = dax_dom_node_get_next_sibling (desc);
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
    DaxDomDocument *document;
    DaxDomNode *svg, *polyline;
    DaxKnotSequence *seq;
    const gfloat *array;
    ClutterUnits *units;

    document = dax_dom_document_new_from_file ("09_06.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));
    /* let's test <svg> width and height attribute here as 09_06.svg had them
     * with height != width */
    g_object_get (G_OBJECT (svg), "width", &units, NULL);
    g_assert_cmpint (clutter_units_get_unit_type (units), ==, CLUTTER_UNIT_CM);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 12.0f);
    g_object_get (G_OBJECT (svg), "height", &units, NULL);
    g_assert_cmpint (clutter_units_get_unit_type (units), ==, CLUTTER_UNIT_CM);
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 4.0f);


    polyline = dax_dom_node_get_last_child (svg);
    g_assert (polyline);
    g_assert (DAX_IS_ELEMENT_POLYLINE (polyline));

    g_object_get (G_OBJECT (polyline), "points", &seq, NULL);
    g_assert (dax_knot_sequence_get_size (seq) == 22);
    array = dax_knot_sequence_get_array (seq);
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
    DaxDomDocument *document;
    DaxDomNode *svg, *path;
    ClutterPath *clutter_path;
    ClutterPathNode node;

    document = dax_dom_document_new_from_file ("08_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    path = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_PATH (path));
    clutter_path = dax_element_path_get_path (DAX_ELEMENT_PATH (path));
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
    DaxDomDocument *document;
    DaxDomNode *svg, *rect, *animate;
    DaxAnimationAttributeType attribute_type;
    DaxDuration *duration;
    DaxRepeatCount *count;
    gchar *string;

    document = dax_dom_document_new_from_file ("19_011.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    rect = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_RECT (rect));

    animate = dax_dom_node_get_last_child (rect);
    g_assert (DAX_IS_ELEMENT_ANIMATE (animate));
    g_object_get (animate, "attributeType", &attribute_type, NULL);
    g_assert_cmpint (attribute_type, ==, DAX_ANIMATION_ATTRIBUTE_TYPE_CSS);
    g_object_get (animate, "attributeName", &string, NULL);
    g_assert_cmpstr (string, ==, "fill-opacity");
    g_object_get (animate, "from", &string, NULL);
    g_assert_cmpstr (string, ==, "1");
    g_object_get (animate, "to", &string, NULL);
    g_assert_cmpstr (string, ==, "0");
    g_object_get (animate, "dur", &duration, NULL);
    g_assert_cmpfloat (dax_duration_to_ms (duration), ==, 5000.f);
    g_object_get (animate, "repeatCount", &count, NULL);
    g_assert (dax_repeat_count_is_indefinite (count));
}

static void
test_title_desc (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *title, *desc, *text;

    document = dax_dom_document_new_from_file ("08_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <title> */
    title = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_TITLE (title));

    /* text node of <title> */
    text = dax_dom_node_get_first_child (title);
    g_assert (DAX_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        dax_dom_character_data_get_data (DAX_DOM_CHARACTER_DATA (text)),
        ==,
        "Example triangle01- simple example of a 'path'");

    /* <desc> */
    desc = dax_dom_node_get_next_sibling (title);
    g_assert (DAX_IS_ELEMENT_DESC (desc));

    /* text node of <desc> */
    text = dax_dom_node_get_first_child (desc);
    g_assert (DAX_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        dax_dom_character_data_get_data (DAX_DOM_CHARACTER_DATA (text)),
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
    DaxDomDocument *document;
    DaxDomNode *svg, *desc, *script, *text;
    DaxScriptType type;

    document = dax_dom_document_new_from_file ("18_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (desc));

    /* <script> */
    script = dax_dom_node_get_next_sibling (desc);
    g_assert (DAX_IS_ELEMENT_SCRIPT (script));
    g_object_get (script, "type", &type, NULL);
    g_assert_cmpint (type, ==, DAX_SCRIPT_TYPE_ECMASCRIPT);

    /* text node of <script> */
    text = dax_dom_node_get_first_child (script);
    g_assert (DAX_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        dax_dom_character_data_get_data (DAX_DOM_CHARACTER_DATA (text)),
        ==,
        _18_01_script);
}

static void
test_circle (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *circle;
    DaxScriptType type;
    ClutterUnits *units;

    document = dax_dom_document_new_from_file ("09_03.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <circle> */
    circle = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_CIRCLE (circle));
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
    DaxDomDocument *document;
    DaxDomNode *svg, *text, *circle, *handler, *text_node;
    DaxScriptType type;
    DaxXmlEventType event_type;

    document = dax_dom_document_new_from_file ("18_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <text> */
    text = dax_dom_node_get_last_child (svg);
#if 0
    FIXME enable this when the text element is parsed
    g_assert (DAX_IS_TEXT_ELEMENT (text));
#endif

    /* <circle> */
    circle = dax_dom_node_get_previous_sibling (text);
    g_assert (DAX_IS_ELEMENT_CIRCLE (circle));

    /* <handler> */
    handler = dax_dom_node_get_first_child (circle);
    g_assert (DAX_IS_ELEMENT_HANDLER (handler));
    g_object_get (handler, "type", &type, NULL);
    g_assert_cmpint (type, ==, DAX_SCRIPT_TYPE_ECMASCRIPT);
    g_object_get (handler, "event", &event_type, NULL);
    g_assert_cmpint (event_type, ==, DAX_XML_EVENT_TYPE_CLICK);

    /* text node of <handler> */
    text_node = dax_dom_node_get_first_child (handler);
    g_assert (DAX_IS_DOM_TEXT (text_node));
    g_assert_cmpstr (
        dax_dom_character_data_get_data (DAX_DOM_CHARACTER_DATA (text_node)),
        ==,
        _18_01_handler);
}

static void
test_line (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *g, *line;
    ClutterUnits *units;

    document = dax_dom_document_new_from_file ("09_05.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <g> */
    g = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_G (g));

    /* <line> */
    line = dax_dom_node_get_first_child (g);
    g_assert (DAX_IS_ELEMENT_LINE (line));

    units = dax_element_line_get_x1 (DAX_ELEMENT_LINE (line));
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 100.0f);
    units = dax_element_line_get_y1 (DAX_ELEMENT_LINE (line));
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 300.0f);
    units = dax_element_line_get_x2 (DAX_ELEMENT_LINE (line));
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 300.0f);
    units = dax_element_line_get_y2 (DAX_ELEMENT_LINE (line));
    g_assert_cmpfloat (clutter_units_get_unit_value(units), ==, 100.0f);
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    dax_init (&argc, &argv);

    g_test_add_func ("/parser/simple-document-from-file",
                     test_simple_document_from_file);
    g_test_add_func ("/parser/polyline", test_polyline);
    g_test_add_func ("/parser/path", test_path);
    g_test_add_func ("/parser/animate", test_animate);
    g_test_add_func ("/parser/title-desc", test_title_desc);
    g_test_add_func ("/parser/script", test_script);
    g_test_add_func ("/parser/circle", test_circle);
    g_test_add_func ("/parser/handler", test_handler);
    g_test_add_func ("/parser/line", test_line);

    return g_test_run ();
}
