/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#ifndef __DAX_DOM_NODE_H__
#define __DAX_DOM_NODE_H__

#include <glib-object.h>

#include "dax-dom-forward.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOM_NODE dax_dom_node_get_type()

#define DAX_DOM_NODE(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 DAX_TYPE_DOM_NODE,  \
                                 DaxDomNode))

#define DAX_DOM_NODE_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              DAX_TYPE_DOM_NODE, \
                              DaxDomNodeClass))

#define DAX_IS_DOM_NODE(obj)         \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_DOM_NODE))

#define DAX_IS_DOM_NODE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),  \
                              DAX_TYPE_DOM_NODE))

#define DAX_DOM_NODE_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_DOM_NODE,   \
                                DaxDomNodeClass))

typedef struct _DaxDomNodeClass DaxDomNodeClass;
typedef struct _DaxDomNodePrivate DaxDomNodePrivate;

/**
 * DaxDomNodeType: The type of a #DaxDomNode
 */
typedef enum /*< skip >*/
{
    DAX_DOM_NODE_TYPE_NONE,                    /* not part of DOM level 3 */
    DAX_DOM_NODE_TYPE_ELEMENT,
    DAX_DOM_NODE_TYPE_ATTRIBUTE,
    DAX_DOM_NODE_TYPE_TEXT_NODE,
    DAX_DOM_NODE_TYPE_CDATA_SECTION,
    DAX_DOM_NODE_TYPE_ENTITY_REFERENCE,
    DAX_DOM_NODE_TYPE_ENTITY,
    DAX_DOM_NODE_TYPE_PROCESSING_INSTRUCTION,
    DAX_DOM_NODE_TYPE_COMMENT,
    DAX_DOM_NODE_TYPE_DOCUMENT,
    DAX_DOM_NODE_TYPE_DOCUMENT_TYPE,
    DAX_DOM_NODE_TYPE_DOCUMENT_FRAGMENT,
    DAX_DOM_NODE_TYPE_NOTATION,
    DAX_DOM_NODE_TYPE_WHITESPACE,              /* not part of DOM level 3 */
    DAX_DOM_NODE_TYPE_SIGNIFICANT_WHITESPACE,  /* not part of DOM level 3 */
    DAX_DOM_NODE_TYPE_END_ELEMENT,             /* not part of DOM level 3 */
    DAX_DOM_NODE_TYPE_END_ENTITY,              /* not part of DOM level 3 */
    DAX_DOM_NODE_TYPE_XML_DECLARATION,         /* not part of DOM level 3 */
} DaxDomNodeType;

struct _DaxDomNode
{
    GObject parent;

    /*< private >*/
    DaxDomNodePrivate *priv;

    /* this fields are private and should be accessed with the appropriate
     * API. They are here so internal functions can access them without the
     * function call overhead */
    DaxDomNodeType node_type;

    const gchar *namespace_uri; /* interned string */
    DaxDomDocument *owner_document;
    gchar *text_content;    /* FIXME this is bad, text_content has to be
                               build using children text nodes */

    DaxDomNode *parent_node;
    DaxDomNode *first_child;
    DaxDomNode *last_child;
    DaxDomNode *previous_sibling;
    DaxDomNode *next_sibling;
};

struct _DaxDomNodeClass
{
    GObjectClass parent_class;

    DaxDomNodeType  (* get_node_type)   (DaxDomNode *self);
    const gchar       *(* get_local_name)  (DaxDomNode *self);
    const gchar       *(* get_node_name)   (DaxDomNode *self);
};

GType              dax_dom_node_get_type             (void) G_GNUC_CONST;

DaxDomNode     *dax_dom_node_new                  (void);

/* DOM readonly attributes */

DaxDomNodeType  dax_dom_node_get_node_type        (DaxDomNode *self);
const gchar       *dax_dom_node_get_namespace_uri    (DaxDomNode *self);
const gchar       *dax_dom_node_get_node_name        (DaxDomNode *self);
const gchar       *dax_dom_node_get_local_name       (DaxDomNode *self);
DaxDomDocument *dax_dom_node_get_owner_document   (DaxDomNode *self);
DaxDomNode     *dax_dom_node_get_parent_node      (DaxDomNode *self);
DaxDomNode     *dax_dom_node_get_first_child      (DaxDomNode *self);
DaxDomNode     *dax_dom_node_get_last_child       (DaxDomNode *self);
DaxDomNode     *dax_dom_node_get_previous_sibling (DaxDomNode *self);
DaxDomNode     *dax_dom_node_get_next_sibling     (DaxDomNode *self);

/* DOM attributes */

const gchar*       dax_dom_node_get_text_content     (DaxDomNode *self);
void               dax_dom_node_set_text_content     (DaxDomNode *self,
                                                         const gchar   *text_content);

/* DOM methods */

DaxDomNode     *dax_dom_node_append_child         (DaxDomNode  *self,
                                                         DaxDomNode  *new_child,
                                                         GError        **err);
DaxDomNode     *dax_dom_node_insert_before       (DaxDomNode  *self,
                                                        DaxDomNode  *new_child,
                                                        DaxDomNode  *ref_child,
                                                        GError        **err);
DaxDomNode     *dax_dom_node_remove_child        (DaxDomNode  *self,
                                                        DaxDomNode  *old_child,
                                                        GError        **err);
DaxDomNode     *dax_dom_node_clone_node          (DaxDomNode  *self,
                                                        const gboolean  deep);
G_END_DECLS

#endif /* __DAX_DOM_NODE_H__ */
