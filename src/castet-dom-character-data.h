/*
 * Castet - Load and draw SVG
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
/* castet-dom-character-data.h */

#ifndef __CASTET_DOM_CHARACTER_DATA_H__
#define __CASTET_DOM_CHARACTER_DATA_H__

#include <glib-object.h>

#include "castet-dom-node.h"

G_BEGIN_DECLS

#define CASTET_TYPE_DOM_CHARACTER_DATA castet_dom_character_data_get_type()

#define CASTET_DOM_CHARACTER_DATA(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                             \
                                 CASTET_TYPE_DOM_CHARACTER_DATA,    \
                                 CastetDomCharacterData))

#define CASTET_DOM_CHARACTER_DATA_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                          \
                              CASTET_TYPE_DOM_CHARACTER_DATA,   \
                              CastetDomCharacterDataClass))

#define CASTET_IS_DOM_CHARACTER_DATA(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_DOM_CHARACTER_DATA))

#define CASTET_IS_DOM_CHARACTER_DATA_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_DOM_CHARACTER_DATA))

#define CASTET_DOM_CHARACTER_DATA_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                CASTET_TYPE_DOM_CHARACTER_DATA, \
                                CastetDomCharacterDataClass))

typedef struct _CastetDomCharacterData CastetDomCharacterData;
typedef struct _CastetDomCharacterDataClass CastetDomCharacterDataClass;
typedef struct _CastetDomCharacterDataPrivate CastetDomCharacterDataPrivate;

struct _CastetDomCharacterData
{
    CastetDomNode parent;

    CastetDomCharacterDataPrivate *priv;
};

struct _CastetDomCharacterDataClass
{
    CastetDomNodeClass parent_class;
};

GType castet_dom_character_data_get_type (void) G_GNUC_CONST;

CastetDomCharacterData *castet_dom_character_data_new      (void);
void                    castet_dom_character_data_set_data (CastetDomCharacterData *char_data,
                                                            const gchar            *data);
const gchar *           castet_dom_character_data_get_data (CastetDomCharacterData *char_data);

G_END_DECLS

#endif /* __CASTET_DOM_CHARACTER_DATA_H__ */
