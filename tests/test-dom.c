
#include <glib.h>

#include <dax.h>

static const gchar svg_ns[] = "http://www.w3.org/2000/svg";

static void
test_dom_node (void)
{
    DaxDomDocument *document, *temp_doc;
    DaxDomNode *svg, *desc, *rect, *temp;
    const gchar *ns_uri;

    document = dax_dom_document_new_from_file ("01_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));
    ns_uri = dax_dom_node_get_namespace_uri (svg);
    g_assert_cmpstr (ns_uri, ==, svg_ns);
    temp = dax_dom_node_get_parent_node (svg);
    g_assert (temp == DAX_DOM_NODE (document));
    temp = dax_dom_node_get_previous_sibling (svg);
    g_assert (temp == NULL);
    temp = dax_dom_node_get_next_sibling (svg);
    g_assert (temp == NULL);

#if 0
    temp_doc = dax_dom_node_get_owner_document (svg);
    g_assert (temp_doc == document);
#endif

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (desc));
    temp = dax_dom_node_get_parent_node (desc);
    g_assert (temp == svg);

    /* <rect> */
    rect = dax_dom_node_get_last_child (svg);
    g_assert (DAX_IS_ELEMENT_RECT (rect));
    temp = dax_dom_node_get_parent_node (rect);
    g_assert (temp == svg);
    temp = dax_dom_node_get_previous_sibling (rect);
    g_assert (temp == desc);

    temp = dax_dom_node_get_next_sibling (desc);
    g_assert (temp == rect);
}

static void
test_dom_text (void)
{
    DaxDomDocument *document;
    DaxDomNode *svg, *desc, *text;

    document = dax_dom_document_new_from_file ("01_01.svg", NULL);
    g_assert (DAX_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = DAX_DOM_NODE (dax_dom_document_get_document_element (document));
    g_assert (DAX_IS_ELEMENT_SVG (svg));

    /* <desc> */
    desc = dax_dom_node_get_first_child (svg);
    g_assert (DAX_IS_ELEMENT_DESC (desc));

    /* text node of <desc> */
    text = dax_dom_node_get_first_child (desc);
    g_assert (DAX_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        dax_dom_character_data_get_data (DAX_DOM_CHARACTER_DATA (text)),
        ==,
        "Example SVG file");
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    dax_init (&argc, &argv);

    g_test_add_func ("/dom/node", test_dom_node);
    g_test_add_func ("/dom/text", test_dom_text);

    return g_test_run ();
}
