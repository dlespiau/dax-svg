/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include <gio/gio.h>

#include "dax-utils.h"
#include "dax-dom-element.h"

#include "dax-cache.h"

G_DEFINE_TYPE (DaxCache, dax_cache, G_TYPE_OBJECT)

#define CACHE_PRIVATE(o) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), DAX_TYPE_CACHE, DaxCachePrivate))

struct _DaxCachePrivate
{
    GHashTable *entries;    /* "uri" -> DaxCacheEntry map */
};

/*
 * GObject implementation
 */

static void
dax_cache_get_property (GObject    *object,
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
dax_cache_set_property (GObject      *object,
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
dax_cache_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_cache_parent_class)->dispose (object);
}

static void
dax_cache_finalize (GObject *object)
{
    DaxCache *cache = (DaxCache *) object;
    DaxCachePrivate *priv = cache->priv;

    g_hash_table_unref (priv->entries);

    G_OBJECT_CLASS (dax_cache_parent_class)->finalize (object);
}

static void
dax_cache_class_init (DaxCacheClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxCachePrivate));

    object_class->get_property = dax_cache_get_property;
    object_class->set_property = dax_cache_set_property;
    object_class->dispose = dax_cache_dispose;
    object_class->finalize = dax_cache_finalize;
}

static void
dax_cache_init (DaxCache *self)
{
    DaxCachePrivate *priv;

    self->priv = priv = CACHE_PRIVATE (self);

    priv->entries = g_hash_table_new (g_str_hash, g_str_equal);
}

DaxCache *
dax_cache_new (void)
{
    return g_object_new (DAX_TYPE_CACHE, NULL);
}

DaxCache *
dax_cache_get_default (void)
{
    static DaxCache *singleton;

    if (G_UNLIKELY (singleton == NULL)) {
        singleton = dax_cache_new ();
    }

    return singleton;
}

DaxCacheEntry *
dax_cache_get_entry_for_uri (DaxCache      *cache,
                             const gchar   *uri)
{
    DaxCachePrivate *priv;
    DaxCacheEntry *entry;

    g_return_val_if_fail (DAX_IS_CACHE (cache), NULL);
    priv = cache->priv;

    entry = _dax_cache_entry_new (cache, uri);
    g_hash_table_insert (priv->entries, (gchar *) uri, entry);

    return entry;
}

DaxCacheEntry *
dax_cache_get_entry_for_href (DaxCache      *cache,
                              DaxDomElement *element,
                              const gchar   *href)
{
    DaxCachePrivate *priv;
    DaxCacheEntry *entry;
    const gchar *base_iri;
    gchar *resolved_iri;
    GFile *base_file, *resolved_file;

    g_return_val_if_fail (DAX_IS_CACHE (cache), NULL);
    priv = cache->priv;

    if (_dax_utils_is_iri (href)) {
        entry = _dax_cache_entry_new (cache, href);
        g_hash_table_insert (priv->entries, (gchar *) href, entry);
    } else {
        base_iri = dax_dom_element_get_base_iri (element);
        base_file = g_file_new_for_uri (base_iri);

        resolved_file = g_file_resolve_relative_path (base_file, href);
        resolved_iri = g_file_get_uri (resolved_file);

        entry = _dax_cache_entry_new (cache, resolved_iri);
        g_hash_table_insert (priv->entries, (gchar *) resolved_iri, entry);

        g_object_unref (base_file);
        g_object_unref (resolved_file);
        g_free (resolved_iri);
    }

    return entry;
}
