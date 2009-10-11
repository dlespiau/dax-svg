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
/* castet-dom-text.h */

#ifndef __CASTET_DOM_TEXT_H__
#define __CASTET_DOM_TEXT_H__

#include <glib-object.h>

#include "castet-dom-character-data.h"

G_BEGIN_DECLS

#define CASTET_TYPE_DOM_TEXT castet_dom_text_get_type()

#define CASTET_DOM_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), CASTET_TYPE_DOM_TEXT, CastetDomText))

#define CASTET_DOM_TEXT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              CASTET_TYPE_DOM_TEXT, \
                              CastetDomTextClass))

#define CASTET_IS_DOM_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_DOM_TEXT))

#define CASTET_IS_DOM_TEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_DOM_TEXT))

#define CASTET_DOM_TEXT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_DOM_TEXT,   \
                                CastetDomTextClass))

typedef struct _CastetDomText CastetDomText;
typedef struct _CastetDomTextClass CastetDomTextClass;
typedef struct _CastetDomTextPrivate CastetDomTextPrivate;

struct _CastetDomText
{
    CastetDomCharacterData parent;

    CastetDomTextPrivate *priv;
};

struct _CastetDomTextClass
{
    CastetDomCharacterDataClass parent_class;
};

GType castet_dom_text_get_type (void) G_GNUC_CONST;

CastetDomText *castet_dom_text_new (void);

G_END_DECLS

#endif /* __CASTET_DOM_TEXT_H__ */
