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

#ifndef __DAX_CACHE_ENTRY_H__
#define __DAX_CACHE_ENTRY_H__

#include <glib-object.h>

#include "dax-dom-forward.h"

G_BEGIN_DECLS

#define DAX_TYPE_CACHE_ENTRY dax_cache_entry_get_type()

#define DAX_CACHE_ENTRY(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_CACHE_ENTRY, DaxCacheEntry))

#define DAX_CACHE_ENTRY_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              DAX_TYPE_CACHE_ENTRY, \
                              DaxCacheEntryClass))

#define DAX_IS_CACHE_ENTRY(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_CACHE_ENTRY))

#define DAX_IS_CACHE_ENTRY_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_CACHE_ENTRY))

#define DAX_CACHE_ENTRY_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_CACHE_ENTRY,   \
                                DaxCacheEntryClass))

typedef struct _DaxCacheEntryClass DaxCacheEntryClass;
typedef struct _DaxCacheEntryPrivate DaxCacheEntryPrivate;

struct _DaxCacheEntry
{
    GObject parent;

    DaxCacheEntryPrivate *priv;
};

struct _DaxCacheEntryClass
{
    GObjectClass parent_class;
};

GType               dax_cache_entry_get_type        (void) G_GNUC_CONST;

DaxCacheEntry *     _dax_cache_entry_new            (DaxCache    *cache,
                                                     const gchar *uri);
DaxCache *          dax_cache_entry_get_cache       (DaxCacheEntry *entry);
const gchar *       dax_cache_entry_get_uri         (DaxCacheEntry *entry);
void                dax_cache_entry_set_uri         (DaxCacheEntry *entry,
                                                     const gchar   *uri);
const gchar *       dax_cache_entry_get_local_uri   (DaxCacheEntry *entry);
gchar *             dax_cache_entry_get_local_path  (const DaxCacheEntry *entry);
gboolean            dax_cache_entry_is_ready        (DaxCacheEntry *entry);

G_END_DECLS

#endif /* __DAX_CACHE_ENTRY_H__ */
