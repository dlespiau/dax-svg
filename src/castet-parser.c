/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <libxml/xmlreader.h>

#include "castet-debug.h"
#include "castet-document.h"
#include "castet-parser.h"

typedef struct _ParserContext ParserContext;

struct _ParserContext {
    xmlTextReaderPtr reader;
    CastetDomNode *current_node;
};

static void
castet_dom_document_read_node(CastetDomDocument *document,
                              ParserContext     *ctx)
{
    int type;

    type = xmlTextReaderNodeType(ctx->reader);
    switch (type) {
    case CASTET_DOM_NODE_TYPE_ELEMENT:
    {
        const xmlChar *local_name = xmlTextReaderConstLocalName(ctx->reader);
        const gchar *name = (const gchar *)local_name;
        CastetDomElement *new_element;
        gboolean is_empty = FALSE;

        new_element = castet_dom_document_create_element (document, name, NULL);

        if (G_UNLIKELY (new_element == NULL)) {
            g_message ("Unsupported element %s", name);
            new_element =
                castet_dom_document_create_element (document, "desc", NULL);
        }

        CASTET_NOTE (PARSING, "append %s to %s",
                     G_OBJECT_TYPE_NAME (new_element),
                     G_OBJECT_TYPE_NAME (ctx->current_node));

        castet_dom_node_append_child (ctx->current_node,
                                      CASTET_DOM_NODE (new_element),
                                      NULL);
        ctx->current_node = CASTET_DOM_NODE (new_element);

        if (xmlTextReaderIsEmptyElement (ctx->reader))
            is_empty = TRUE;

        /* Parse attributes */
        while (xmlTextReaderMoveToNextAttribute (ctx->reader) == 1) {
            const xmlChar *_name = xmlTextReaderConstLocalName (ctx->reader);
            const xmlChar *_value = xmlTextReaderConstValue (ctx->reader);

            castet_dom_element_set_attribute (new_element,
                                              (const gchar *)_name,
                                              (const gchar *)_value,
                                              NULL);
        }

        /* if the element is empty a CASTET_DOM_NODE_TYPE_END_ELEMENT won't
         * be emited, so update current_node here */
        if (is_empty)
            ctx->current_node = ctx->current_node->parent_node;
        break;
    }
    case CASTET_DOM_NODE_TYPE_END_ELEMENT:
        CASTET_NOTE (PARSING,
                     "end of %s", G_OBJECT_TYPE_NAME (ctx->current_node));
        ctx->current_node = ctx->current_node->parent_node;
        break;
    default:
        break;
    }
}

/**
 * wsvg_document_new:
 *
 * Creates a new #CastetDomDocument. FIXME
 *
 * Return value: the newly created #CastetDomDocument instance
 */
CastetDomDocument *
castet_dom_document_new_from_file (const gchar  *filename,
                                   GError    **error)
{
    CastetDomDocument *document;
    ParserContext ctx;
    int ret;

    ctx.reader = xmlNewTextReaderFilename(filename);
    if (ctx.reader == NULL)
        return NULL;

    document = castet_document_new();
    ctx.current_node = CASTET_DOM_NODE (document);

    ret = xmlTextReaderRead(ctx.reader);
    while (ret == 1) {
        castet_dom_document_read_node (document, &ctx);
        ret = xmlTextReaderRead(ctx.reader);
    }
    xmlFreeTextReader(ctx.reader);
    /* FIXME: handle the error case where ret = -1 */

    return document;
}
