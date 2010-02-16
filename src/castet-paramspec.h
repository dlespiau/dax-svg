/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#ifndef __CASTET_PARAMSPEC_H__
#define __CASTET_PARAMSPEC_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * CastetParamSpecs
 */

/**
 * CastetParamFlags:
 * @CASTET_PARAM_PROPERTY: if set the parameter is a SVG property, if not it's
 * a SVG attribute
 * @CASTET_PARAM_ANIMATABLE: the parameter can be animated
 * @CASTET_PARAM_INHERITABLE: the parameter can be inherited from an ancestor
 */
typedef enum /*< skip >*/
{
    CASTET_PARAM_NONE        = 0,
    CASTET_PARAM_PROPERTY    = 1 << 0,
    CASTET_PARAM_ANIMATABLE  = 1 << 1,
    CASTET_PARAM_INHERITABLE = 1 << 2
} CastetParamFlags;

/*
 * CastetParamSpecEnum
 */

typedef struct _CastetParamSpecEnum CastetParamSpecEnum;

/**
 * CastetParamSpecEnum:
 * @parent_instance: private #GParamSpec portion
 * @enum_class: the #GEnumClass for the enum
 * @default_value: default value for the property specified
 *
 * A #GParamSpec derived structure that contains the meta data for enum
 * properties.
 */
struct _CastetParamSpecEnum
{
  GParamSpec        parent_instance;

  GEnumClass       *enum_class;
  gint              default_value;
  CastetParamFlags  flags;
  const gchar      *namespace_uri;
};

/**
 * CASTET_TYPE_PARAM_ENUM:
 *
 * The #GType of #CastetParamSpecEnum.
 */
#define CASTET_TYPE_PARAM_ENUM          (castet_param_enum_get_type ())

/**
 * CASTET_IS_PARAM_SPEC_ENUM:
 * @pspec: a valid #GParamSpec instance
 *
 * Checks whether the given #GParamSpec is of type %CASTET_TYPE_PARAM_ENUM.
 *
 * Returns: %TRUE on success.
 */
#define CASTET_IS_PARAM_SPEC_ENUM(pspec) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), CASTET_TYPE_PARAM_ENUM))

/**
 * CASTET_PARAM_SPEC_ENUM:
 * @pspec: a valid #GParamSpec instance
 *
 * Cast a #GParamSpec instance into a #CastetParamSpecEnum.
 */
#define CASTET_PARAM_SPEC_ENUM(pspec)                           \
          (G_TYPE_CHECK_INSTANCE_CAST ((pspec),                 \
                                       CASTET_TYPE_PARAM_ENUM,  \
                                       CastetParamSpecEnum))

GType               castet_param_enum_get_type      (void) G_GNUC_CONST;
GParamSpec*         castet_param_spec_enum          (const gchar      *name,
                                                     const gchar      *nick,
                                                     const gchar      *blurb,
                                                     GType             enum_type,
                                                     gint              default_value,
                                                     GParamFlags       g_flags,
                                                     CastetParamFlags  castet_flags,
                                                     const gchar      *namespace_uri);

G_END_DECLS

#endif /* __CASTET_PARAMSPEC_H__ */
