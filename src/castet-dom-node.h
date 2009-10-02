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

#ifndef __CASTET_DOM_NODE_H__
#define __CASTET_DOM_NODE_H__

#include <glib-object.h>

#include "castet-dom-forward.h"

G_BEGIN_DECLS

#define CASTET_TYPE_DOM_NODE castet_dom_node_get_type()

#define CASTET_DOM_NODE(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                 \
                                 CASTET_TYPE_DOM_NODE,  \
                                 CastetDomNode))

#define CASTET_DOM_NODE_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              CASTET_TYPE_DOM_NODE, \
                              CastetDomNodeClass))

#define CASTET_IS_DOM_NODE(obj)         \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_DOM_NODE))

#define CASTET_IS_DOM_NODE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),  \
                              CASTET_TYPE_DOM_NODE))

#define CASTET_DOM_NODE_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_DOM_NODE,   \
                                CastetDomNodeClass))

typedef struct _CastetDomNodeClass CastetDomNodeClass;
typedef struct _CastetDomNodePrivate CastetDomNodePrivate;

/**
 * CastetDomNodeType: The type of a #CastetDomNode
 */
enum _CastetDomNodeType {
    CASTET_DOM_NODE_TYPE_NONE,                    /* not part of DOM level 3 */
    CASTET_DOM_NODE_TYPE_ELEMENT,
    CASTET_DOM_NODE_TYPE_ATTRIBUTE,
    CASTET_DOM_NODE_TYPE_TEXT_NODE,
    CASTET_DOM_NODE_TYPE_CDATA_SECTION,
    CASTET_DOM_NODE_TYPE_ENTITY_REFERENCE,
    CASTET_DOM_NODE_TYPE_ENTITY,
    CASTET_DOM_NODE_TYPE_PROCESSING_INSTRUCTION,
    CASTET_DOM_NODE_TYPE_COMMENT,
    CASTET_DOM_NODE_TYPE_DOCUMENT,
    CASTET_DOM_NODE_TYPE_DOCUMENT_TYPE,
    CASTET_DOM_NODE_TYPE_DOCUMENT_FRAGMENT,
    CASTET_DOM_NODE_TYPE_NOTATION,
    CASTET_DOM_NODE_TYPE_WHITESPACE,              /* not part of DOM level 3 */
    CASTET_DOM_NODE_TYPE_SIGNIFICANT_WHITESPACE,  /* not part of DOM level 3 */
    CASTET_DOM_NODE_TYPE_END_ELEMENT,             /* not part of DOM level 3 */
    CASTET_DOM_NODE_TYPE_END_ENTITY,              /* not part of DOM level 3 */
    CASTET_DOM_NODE_TYPE_XML_DECLARATION,         /* not part of DOM level 3 */
};
typedef enum _CastetDomNodeType CastetDomNodeType;

struct _CastetDomNode
{
    GObject parent;

    /*< private >*/
    CastetDomNodePrivate *priv;

    /* this fields are private and should be accessed with the appropriate
     * API. They are here so internals functions can access them without the
     * function call overhead */
    CastetDomNodeType node_type;

    gchar *namespace_uri;
    CastetDomDocument *owner_document;
    gchar *text_content;

    CastetDomNode *parent_node;
    CastetDomNode *first_child;
    CastetDomNode *last_child;
    CastetDomNode *previous_sibling;
    CastetDomNode *next_sibling;
};

struct _CastetDomNodeClass
{
    GObjectClass parent_class;

    CastetDomNodeType  (* get_node_type)   (CastetDomNode *self);
    const gchar       *(* get_local_name)  (CastetDomNode *self);
    const gchar       *(* get_node_name)   (CastetDomNode *self);
};

GType              castet_dom_node_get_type          (void) G_GNUC_CONST;

CastetDomNode     *castet_dom_node_new               (void);

/* DOM readonly attributes */

CastetDomNodeType  castet_dom_node_get_node_type        (CastetDomNode *self);
const char*        castet_dom_node_get_namespace_uri    (CastetDomNode *self);
const char*        castet_dom_node_get_node_name        (CastetDomNode *self);
const char*        castet_dom_node_get_local_name       (CastetDomNode *self);
CastetDomDocument *castet_dom_node_get_owner_document   (CastetDomNode *self);
CastetDomNode*     castet_dom_node_get_parent_node      (CastetDomNode *self);
CastetDomNode*     castet_dom_node_get_first_child      (CastetDomNode *self);
CastetDomNode*     castet_dom_node_get_last_child       (CastetDomNode *self);
CastetDomNode*     castet_dom_node_get_previous_sibling (CastetDomNode *self);
CastetDomNode*     castet_dom_node_get_next_sibling     (CastetDomNode *self);

/* DOM attributes */

const char*        castet_dom_node_get_text_content     (CastetDomNode *self);
void               castet_dom_node_set_text_content     (CastetDomNode *self,
                                                         const char    *text_content);

/* DOM methods */

CastetDomNode*      castet_dom_node_append_child        (CastetDomNode  *self,
                                                         CastetDomNode  *new_child,
                                                         GError        **err);
CastetDomNode*      castet_dom_node_insert_before       (CastetDomNode  *self,
                                                         CastetDomNode  *new_child,
                                                         CastetDomNode  *ref_child,
                                                         GError        **err);
CastetDomNode*      castet_dom_node_remove_child        (CastetDomNode  *self,
                                                         CastetDomNode  *old_child,
                                                         GError        **err);
CastetDomNode*      castet_dom_node_clone_node          (CastetDomNode  *self,
                                                         const gboolean  deep);
G_END_DECLS

#endif /* __CASTET_DOM_NODE_H__ */
