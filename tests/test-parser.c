/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include <math.h>
#include <string.h>

#include <glib.h>

#include <dax.h>

#include "test-common.h"

static void
test_simple_document_from_file (void)
{
    DaxDomDocument *document;
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
    ClutterPath2D *clutter_path;
    ClutterPath2DNode node;

    document = dax_dom_document_new_from_file ("08_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    path = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_PATH (path));
    clutter_path = dax_element_path_get_path (DAX_ELEMENT_PATH (path));
    g_assert (clutter_path_2d_get_n_nodes (clutter_path) == 4);
    clutter_path_2d_get_node (clutter_path, 0, &node);
    g_assert (node.type == CLUTTER_PATH_MOVE_TO);
    g_assert_cmpint (node.points[0].x, ==, 100);
    g_assert_cmpint (node.points[0].y, ==, 100);
    clutter_path_2d_get_node (clutter_path, 1, &node);
    g_assert (node.type == CLUTTER_PATH_LINE_TO);
    g_assert_cmpint (node.points[0].x, ==, 300);
    g_assert_cmpint (node.points[0].y, ==, 100);
    clutter_path_2d_get_node (clutter_path, 2, &node);
    g_assert (node.type == CLUTTER_PATH_LINE_TO);
    g_assert_cmpint (node.points[0].x, ==, 200);
    g_assert_cmpint (node.points[0].y, ==, 300);
    clutter_path_2d_get_node (clutter_path, 3, &node);
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

static void
test_text (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *desc, *text;
    GArray *array;
    ClutterUnits *units;
    gfloat value;

    document = dax_dom_document_new_from_file ("10_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (desc));

    /* <text> */
    text = dax_dom_node_get_next_sibling (desc);
    g_assert (DAX_IS_ELEMENT_TEXT (text));

    array = dax_element_text_get_x (DAX_ELEMENT_TEXT (text));
    g_assert_cmpint (array->len, ==, 1);
    units = &g_array_index (array, ClutterUnits, 0);
    value = clutter_units_get_unit_value (units);
    g_assert_cmpfloat (value, ==, 250);

    array = dax_element_text_get_y (DAX_ELEMENT_TEXT (text));
    g_assert_cmpint (array->len, ==, 1);
    units = &g_array_index (array, ClutterUnits, 0);
    value = clutter_units_get_unit_value (units);
    g_assert_cmpfloat (value, ==, 150);
}

static void
test_image (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *desc, *image_node;
    DaxElementImage *image;
    ClutterUnits *units;
    gfloat value;

    document = dax_dom_document_new_from_file ("05_21.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (desc));

    /* <image> */
    image_node = dax_dom_node_get_next_sibling (desc);
    g_assert (DAX_IS_ELEMENT_IMAGE (image_node));
    image = DAX_ELEMENT_IMAGE (image_node);

    units = dax_element_image_get_x (image);
    value = clutter_units_to_pixels (units);
    g_assert_cmpfloat (value, ==, 200.0f);
    units = dax_element_image_get_y (image);
    value = clutter_units_to_pixels (units);
    g_assert_cmpfloat (value, ==, 200.0f);
    units = dax_element_image_get_width (image);
    value = clutter_units_to_pixels (units);
    g_assert_cmpfloat (value, ==, 100.0f);
    units = dax_element_image_get_height (image);
    value = clutter_units_to_pixels (units);
    g_assert_cmpfloat (value, ==, 100.0f);
}

static void
test_base (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *g1, *g2, *g3;
    const gchar *iri;

    document = dax_dom_document_new_from_memory (image_use_base,
                                                 sizeof (image_use_base) - 1,
                                                 "http://www.example.com",
                                                 NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));
    iri = dax_dom_element_get_base_iri (DAX_DOM_ELEMENT (svg));
    g_assert_cmpstr (iri, ==, "http://www.example.com");

    /* <g> */
    g1 = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_G (g1));
    iri = dax_dom_element_get_base_iri (DAX_DOM_ELEMENT (g1));
    g_assert_cmpstr (iri, ==, "http://a.example.org/aaa/");

    /* <g> */
    g2 = dax_dom_node_get_first_child (g1);
    g_assert (DAX_IS_ELEMENT_G (g2));
    iri = dax_dom_element_get_base_iri (DAX_DOM_ELEMENT (g2));
    g_assert_cmpstr (iri, ==, "http://a.example.org/bbb/ccc/");

    /* <g> */
    g3 = dax_dom_node_get_first_child (g2);
    g_assert (DAX_IS_ELEMENT_G (g3));
    iri = dax_dom_element_get_base_iri (DAX_DOM_ELEMENT (g3));
    /* glib <= 2.24.0 don't remove the dot segments in GDummyFile */
    g_assert (strcmp (iri, "http://a.example.org/bbb/ddd/") == 0 ||
              strcmp (iri, "http://a.example.org/bbb/ccc/../ddd/") == 0);

    /* check we've parsed the id */
    g_assert_cmpstr (dax_dom_element_get_id (DAX_DOM_ELEMENT (g3)),
                     ==,
                     "bar");
}

static struct {
    const gchar *str;
    DaxPreserveAspectRatioAlign align;
} test_par [10] = {
    { "none", DAX_PRESERVE_ASPECT_RATIO_ALIGN_NONE },
    { "xMinYMin", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MIN },
    { "xMidYMin", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MIN },
    { "xMaxYMin", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MIN },
    { "xMinYMid", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MID },
    { "xMidYMid", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MID },
    { "xMaxYMid", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MID },
    { "xMinYMax", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MIN_Y_MAX },
    { "xMidYMax", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MID_Y_MAX },
    { "xMaxYMax", DAX_PRESERVE_ASPECT_RATIO_ALIGN_X_MAX_Y_MAX }
};

static void
test_preserve_ar (void)
{
    DaxPreserveAspectRatio par;
    gboolean ret;
    GString *str;
    guint i;

    for (i = 0; i < G_N_ELEMENTS (test_par); i++) {
        ret = dax_preserve_aspect_ratio_from_string (&par, test_par[i].str);
        g_assert (ret);
        g_assert_cmpint (par.align, ==, test_par[i].align);
        g_assert (!dax_preserve_aspect_ratio_has_defer (&par));
        g_assert (!dax_preserve_aspect_ratio_has_meet (&par));
    }

    str = g_string_new (NULL);
    g_string_append_printf (str, "defer    %s", test_par[7].str);
    ret = dax_preserve_aspect_ratio_from_string (&par, str->str);
    g_assert (ret);
    g_assert_cmpint (par.align, ==, test_par[7].align);
    g_assert (dax_preserve_aspect_ratio_has_defer (&par));
    g_assert (!dax_preserve_aspect_ratio_has_meet (&par));
    g_string_free (str, TRUE);

    str = g_string_new (NULL);
    g_string_append_printf (str, " %s  meet", test_par[8].str);
    ret = dax_preserve_aspect_ratio_from_string (&par, str->str);
    g_assert (ret);
    g_assert_cmpint (par.align, ==, test_par[8].align);
    g_assert (!dax_preserve_aspect_ratio_has_defer (&par));
    g_assert (dax_preserve_aspect_ratio_has_meet (&par));
    g_string_free (str, TRUE);
}

#define fill_matrix(mat, a, b, c, d, e, f)      \
    do {                                        \
        mat.affine[0] = a; mat.affine[1] = b;   \
        mat.affine[2] = c; mat.affine[3] = d;   \
        mat.affine[4] = e; mat.affine[5] = f;   \
    } while (0)

#define EPSILON 1e-6

static gboolean
_dax_affine_equal (double matrix1[6],
                   double matrix2[6])
{
    return (fabs (matrix1[0] - matrix2[0]) < EPSILON &&
            fabs (matrix1[1] - matrix2[1]) < EPSILON &&
            fabs (matrix1[2] - matrix2[2]) < EPSILON &&
            fabs (matrix1[3] - matrix2[3]) < EPSILON &&
            fabs (matrix1[4] - matrix2[4]) < EPSILON &&
            fabs (matrix1[5] - matrix2[5]) < EPSILON);
}

static void
dax_assert_matrix_equal (DaxMatrix *m1,
                         DaxMatrix *m2)
{
    gboolean res;

    res = _dax_affine_equal (m1->affine, m2->affine);
    if (res == FALSE) {
        g_message ("[%.02lf %.02lf %.02lf %.02lf %.02lf %.02lf] != "
                   "[%.02lf %.02lf %.02lf %.02lf %.02lf %.02lf]",
                   m1->affine[0], m1->affine[1], m1->affine[2],
                   m1->affine[3], m1->affine[4], m1->affine[5],
                   m2->affine[0], m2->affine[1], m2->affine[2],
                   m2->affine[3], m2->affine[4], m2->affine[5]);
    }
    g_assert (res);
}

static void
test_transform (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *tmp, *g;
    DaxMatrix *matrix;
    DaxMatrix translate1, rotate1, translate2;
    double s, c;

    fill_matrix (translate1, 1, 0, 0, 1, 50, 90);
    s = sin (-45 * M_PI / 180.0);
    c = cos (-45 * M_PI / 180.0);
    fill_matrix (rotate1, c, s, -s, c, 0, 0);
    fill_matrix (translate2, 1, 0, 0, 1, 130, 160);

    document = dax_dom_document_new_from_file ("07_07.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <desc> */
    tmp = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (tmp));

    /* <g> */
    g = dax_dom_node_get_next_sibling (tmp);
    g_assert (DAX_IS_ELEMENT_G (g));

    /* <g> */
    g = dax_dom_node_get_next_sibling (g);
    g_assert (DAX_IS_ELEMENT_G (g));
    g_object_get (g, "transform", &matrix, NULL);
    dax_assert_matrix_equal (matrix, &translate1);
    dax_matrix_free (matrix);

    /* <g> */
    tmp = dax_dom_node_get_first_child (g);
    g_assert (DAX_IS_ELEMENT_G (tmp));

    /* <text> */
    tmp = dax_dom_node_get_next_sibling (tmp);
    g_assert (DAX_IS_ELEMENT_TEXT (tmp));

    /* <g> */
    g = dax_dom_node_get_next_sibling (tmp);
    g_assert (DAX_IS_ELEMENT_G (g));
    g_object_get (g, "transform", &matrix, NULL);
    dax_assert_matrix_equal (matrix, &rotate1);
    dax_matrix_free (matrix);

    /* <g> */
    tmp = dax_dom_node_get_first_child (g);
    g_assert (DAX_IS_ELEMENT_G (tmp));

    /* <text> */
    tmp = dax_dom_node_get_next_sibling (tmp);
    g_assert (DAX_IS_ELEMENT_TEXT (tmp));

    /* <g> */
    g = dax_dom_node_get_next_sibling (tmp);
    g_assert (DAX_IS_ELEMENT_G (g));
    g_object_get (g, "transform", &matrix, NULL);
    dax_assert_matrix_equal (matrix, &translate2);
    dax_matrix_free (matrix);
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
    g_test_add_func ("/parser/element/polyline", test_polyline);
    g_test_add_func ("/parser/element/path", test_path);
    g_test_add_func ("/parser/element/animate", test_animate);
    g_test_add_func ("/parser/element/title-desc", test_title_desc);
    g_test_add_func ("/parser/element/script", test_script);
    g_test_add_func ("/parser/element/circle", test_circle);
    g_test_add_func ("/parser/element/handler", test_handler);
    g_test_add_func ("/parser/element/line", test_line);
    g_test_add_func ("/parser/element/text", test_text);
    g_test_add_func ("/parser/element/image", test_image);
    g_test_add_func ("/parser/xml-base", test_base);
    g_test_add_func ("/parser/preserve-aspect-ratio", test_preserve_ar);
    g_test_add_func ("/parser/transform", test_transform);

    return g_test_run ();
}
