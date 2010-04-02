/*
 * Dax - Load and draw SVG
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

#ifndef __DAX_PARAMSPEC_H__
#define __DAX_PARAMSPEC_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * DaxParamSpecs
 */

/**
 * DaxParamFlags:
 * @DAX_PARAM_PROPERTY: if set the parameter is a SVG property, if not it's
 * a SVG attribute
 * @DAX_PARAM_ANIMATABLE: the parameter can be animated
 * @DAX_PARAM_INHERITABLE: the parameter can be inherited from an ancestor
 */
typedef enum /*< skip >*/
{
    DAX_PARAM_NONE        = 0,
    DAX_PARAM_PROPERTY    = 1 << 0,
    DAX_PARAM_ANIMATABLE  = 1 << 1,
    DAX_PARAM_INHERITABLE = 1 << 2
} DaxParamFlags;

/*
 * DaxParamSpecEnum
 */

typedef struct _DaxParamSpecEnum DaxParamSpecEnum;

/**
 * DaxParamSpecEnum:
 *
 * A #GParamSpecEnum derived structure that contains extra meta data for enum
 * properties.
 */
struct _DaxParamSpecEnum
{
  GParamSpecEnum  parent_instance;

  DaxParamFlags   flags;
  const gchar    *namespace_uri;
};

/**
 * DAX_TYPE_PARAM_ENUM:
 *
 * The #GType of #DaxParamSpecEnum.
 */
#define DAX_TYPE_PARAM_ENUM          (dax_param_enum_get_type ())

/**
 * DAX_IS_PARAM_SPEC_ENUM:
 * @pspec: a valid #GParamSpec instance
 *
 * Checks whether the given #GParamSpec is of type %DAX_TYPE_PARAM_ENUM.
 *
 * Returns: %TRUE on success.
 */
#define DAX_IS_PARAM_SPEC_ENUM(pspec) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), DAX_TYPE_PARAM_ENUM))

/**
 * DAX_PARAM_SPEC_ENUM:
 * @pspec: a valid #GParamSpec instance
 *
 * Cast a #GParamSpec instance into a #DaxParamSpecEnum.
 */
#define DAX_PARAM_SPEC_ENUM(pspec)                          \
          (G_TYPE_CHECK_INSTANCE_CAST ((pspec),             \
                                       DAX_TYPE_PARAM_ENUM, \
                                       DaxParamSpecEnum))

GType       dax_param_enum_get_type     (void) G_GNUC_CONST;
GParamSpec* dax_param_spec_enum         (const gchar   *name,
                                         const gchar   *nick,
                                         const gchar   *blurb,
                                         GType          enum_type,
                                         gint           default_value,
                                         GParamFlags    g_flags,
                                         DaxParamFlags  dax_flags,
                                         const gchar   *namespace_uri);

G_END_DECLS

#endif /* __DAX_PARAMSPEC_H__ */
