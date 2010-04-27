/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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
/* dax-dom-text.h */

#ifndef __DAX_DOM_TEXT_H__
#define __DAX_DOM_TEXT_H__

#include <glib-object.h>

#include "dax-dom-character-data.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOM_TEXT dax_dom_text_get_type()

#define DAX_DOM_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_DOM_TEXT, DaxDomText))

#define DAX_DOM_TEXT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              DAX_TYPE_DOM_TEXT, \
                              DaxDomTextClass))

#define DAX_IS_DOM_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_DOM_TEXT))

#define DAX_IS_DOM_TEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_DOM_TEXT))

#define DAX_DOM_TEXT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_DOM_TEXT,   \
                                DaxDomTextClass))

typedef struct _DaxDomText DaxDomText;
typedef struct _DaxDomTextClass DaxDomTextClass;
typedef struct _DaxDomTextPrivate DaxDomTextPrivate;

struct _DaxDomText
{
    DaxDomCharacterData parent;

    DaxDomTextPrivate *priv;
};

struct _DaxDomTextClass
{
    DaxDomCharacterDataClass parent_class;
};

GType dax_dom_text_get_type (void) G_GNUC_CONST;

DaxDomText *dax_dom_text_new (void);

G_END_DECLS

#endif /* __DAX_DOM_TEXT_H__ */
