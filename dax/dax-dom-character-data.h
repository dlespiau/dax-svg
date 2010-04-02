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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */
/* dax-dom-character-data.h */

#ifndef __DAX_DOM_CHARACTER_DATA_H__
#define __DAX_DOM_CHARACTER_DATA_H__

#include <glib-object.h>

#include "dax-dom-node.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOM_CHARACTER_DATA dax_dom_character_data_get_type()

#define DAX_DOM_CHARACTER_DATA(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                             \
                                 DAX_TYPE_DOM_CHARACTER_DATA,    \
                                 DaxDomCharacterData))

#define DAX_DOM_CHARACTER_DATA_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                          \
                              DAX_TYPE_DOM_CHARACTER_DATA,   \
                              DaxDomCharacterDataClass))

#define DAX_IS_DOM_CHARACTER_DATA(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_DOM_CHARACTER_DATA))

#define DAX_IS_DOM_CHARACTER_DATA_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_DOM_CHARACTER_DATA))

#define DAX_DOM_CHARACTER_DATA_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_DOM_CHARACTER_DATA, \
                                DaxDomCharacterDataClass))

typedef struct _DaxDomCharacterData DaxDomCharacterData;
typedef struct _DaxDomCharacterDataClass DaxDomCharacterDataClass;
typedef struct _DaxDomCharacterDataPrivate DaxDomCharacterDataPrivate;

struct _DaxDomCharacterData
{
    DaxDomNode parent;

    DaxDomCharacterDataPrivate *priv;
};

struct _DaxDomCharacterDataClass
{
    DaxDomNodeClass parent_class;
};

GType dax_dom_character_data_get_type (void) G_GNUC_CONST;

DaxDomCharacterData *dax_dom_character_data_new      (void);
void                    dax_dom_character_data_set_data (DaxDomCharacterData *char_data,
                                                            const gchar            *data);
const gchar *           dax_dom_character_data_get_data (DaxDomCharacterData *char_data);

G_END_DECLS

#endif /* __DAX_DOM_CHARACTER_DATA_H__ */
