
#include <glib.h>

#include <castet.h>

static void
test_dom_node (void)
{
    CastetDomDocument *document, *temp_doc;
    CastetDomNode *svg, *desc, *rect, *temp;

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
    g_assert (CASTET_IS_DESC_ELEMENT (desc));
    temp = castet_dom_node_get_parent_node (desc);
    g_assert (temp == svg);

    /* <rect> */
    rect = castet_dom_node_get_last_child (svg);
    g_assert (CASTET_IS_RECT_ELEMENT (rect));
    temp = castet_dom_node_get_parent_node (rect);
    g_assert (temp == svg);
    temp = castet_dom_node_get_previous_sibling (rect);
    g_assert (temp == desc);

    temp = castet_dom_node_get_next_sibling (desc);
    g_assert (temp == rect);
}

static void
test_dom_text (void)
{
    CastetDomDocument *document;
    CastetDomNode *svg, *desc, *text;

    document = castet_dom_document_new_from_file ("01_01.svg", NULL);
    g_assert (CASTET_IS_DOM_DOCUMENT (document));

    /* <svg> */
    svg = CASTET_DOM_NODE (castet_dom_document_get_document_element (document));
    g_assert (CASTET_IS_SVG_ELEMENT (svg));

    /* <desc> */
    desc = castet_dom_node_get_first_child (svg);
    g_assert (CASTET_IS_DESC_ELEMENT (desc));

    /* text node of <desc> */
    text = castet_dom_node_get_first_child (desc);
    g_assert (CASTET_IS_DOM_TEXT (text));
    g_assert_cmpstr (
        castet_dom_character_data_get_data (CASTET_DOM_CHARACTER_DATA (text)),
        ==,
        "Example SVG file");
}

int
main (int   argc,
      char *argv[])
{
    g_type_init ();
    g_test_init (&argc, &argv, NULL);
    castet_init (&argc, &argv);

    g_test_add_func ("/dom/node", test_dom_node);
    g_test_add_func ("/dom/text", test_dom_text);

    return g_test_run ();
}
