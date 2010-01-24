/*
 * Castet - Load and draw SVG
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "castet-internals.h"
#include "castet-dom-node.h"

G_DEFINE_ABSTRACT_TYPE (CastetDomNode, castet_dom_node, G_TYPE_OBJECT)

#define DOM_NODE_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      CASTET_TYPE_DOM_NODE, \
                                      CastetDomNodePrivate))

struct _CastetDomNodePrivate
{
};

/*
 * CastetDomNode
 */

CastetDomNodeType
castet_dom_node_get_node_type(CastetDomNode *self)
{
    CastetDomNodeClass *klass = CASTET_DOM_NODE_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, CASTET_DOM_NODE_TYPE_NONE);

    if (klass->get_node_type)
        return klass->get_node_type(self);

    return CASTET_DOM_NODE_TYPE_NONE;
}

const gchar *
castet_dom_node_get_namespace_uri(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->namespace_uri;
}

const gchar *
castet_dom_node_get_node_name(CastetDomNode *self)
{
    CastetDomNodeClass *klass = CASTET_DOM_NODE_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->get_node_name)
        return klass->get_node_name(self);

    return NULL;
}

const gchar *
castet_dom_node_get_local_name(CastetDomNode *self)
{
    CastetDomNodeClass *klass = CASTET_DOM_NODE_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->get_local_name)
        return klass->get_local_name(self);

    return NULL;
}

CastetDomNode *
castet_dom_node_get_parent_node(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->parent_node;
}

CastetDomNode *
castet_dom_node_get_first_child(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->first_child;
}

CastetDomNode *
castet_dom_node_get_last_child(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->last_child;
}

CastetDomNode *
castet_dom_node_get_previous_sibling(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->previous_sibling;
}

CastetDomNode *
castet_dom_node_get_next_sibling(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->next_sibling;
}

CastetDomDocument *
castet_dom_node_get_owner_document(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->owner_document;
}

/* DOM attributes */

const char *
castet_dom_node_get_text_content(CastetDomNode *self)
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    return self->text_content;
}

void
castet_dom_node_set_text_content(CastetDomNode *self,
                                 const gchar   *unused(text_content))
{
    g_return_if_fail(CASTET_IS_DOM_NODE(self));

    g_warning("Not implemented");
}

/* DOM methods */

CastetDomNode *
castet_dom_node_append_child (CastetDomNode  *self,
                              CastetDomNode  *new_child,
                              GError        **unused(err))
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);
    g_return_val_if_fail (new_child->parent_node == NULL, NULL);

    if (self->first_child == NULL)
        self->first_child = new_child;
    if (self->last_child != NULL)
        self->last_child->next_sibling = new_child;

    new_child->parent_node = self;
    new_child->next_sibling = NULL;
    new_child->previous_sibling = self->last_child;
    self->last_child = new_child;

    return new_child;
}

CastetDomNode *
castet_dom_node_insert_before(CastetDomNode  *self,
                              CastetDomNode  *unused(new_child),
                              CastetDomNode  *unused(ref_child),
                              GError        **unused(err))
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    g_warning("Not implemented");

    return NULL;
}

CastetDomNode *
castet_dom_node_remove_child(CastetDomNode  *self,
                         CastetDomNode  *unused(old_child),
                         GError        **unused(err))
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    g_warning("Not implemented");

    return NULL;
}

CastetDomNode *
castet_dom_node_clone_node(CastetDomNode  *self,
                           const gboolean  unused(deep))
{
    g_return_val_if_fail(CASTET_IS_DOM_NODE(self), NULL);

    g_warning("Not implemented");

    return NULL;
}

/*
 * GObject overloading
 */

static void
castet_dom_node_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_dom_node_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_dom_node_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_node_parent_class)->dispose (object);
}

static void
castet_dom_node_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_node_parent_class)->finalize (object);
}

static void
castet_dom_node_class_init (CastetDomNodeClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (CastetDomNodePrivate)); */

    object_class->get_property = castet_dom_node_get_property;
    object_class->set_property = castet_dom_node_set_property;
    object_class->dispose = castet_dom_node_dispose;
    object_class->finalize = castet_dom_node_finalize;
}

static void
castet_dom_node_init (CastetDomNode *self)
{
    /* self->priv = DOM_NODE_PRIVATE (self); */
}

CastetDomNode *
castet_dom_node_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_NODE, NULL);
}
