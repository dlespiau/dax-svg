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

#include "dax-internals.h"

G_BEGIN_DECLS

/*
 * DaxParamSpecs
 */

typedef struct _DaxParamSpecClass DaxParamSpecClass;

typedef struct _DaxParamSpecEnum   DaxParamSpecEnum;
typedef struct _DaxParamSpecString DaxParamSpecString;
typedef struct _DaxParamSpecBoxed  DaxParamSpecBoxed;
typedef struct _DaxParamSpecArray  DaxParamSpecArray;

#define DAX_PARAM_SPEC_CLASS(pclass) \
    (G_TYPE_CHECK_CLASS_CAST ((pclass), G_TYPE_PARAM, DaxParamSpecClass))

#define DAX_PARAM_SPEC_GET_CLASS(pspec) \
    (G_TYPE_INSTANCE_GET_CLASS (pspec, G_TYPE_PARAM, DaxParamSpecClass))

struct _DaxParamSpecClass
{
    GParamSpecClass parent_class;

    gboolean    (*from_string)  (GParamSpec *pspec,
                                 const char *string,
                                 GValue     *new_value);
    gchar *     (*to_string)    (GParamSpec *pspec,
                                 GValue     *value);
};

/**
 * DaxParamFlags:
 * @DAX_PARAM_PROPERTY: if set the parameter is a SVG property, if not it's
 * a SVG attribute
 * @DAX_PARAM_ANIMATABLE: the parameter can be animated
 * @DAX_PARAM_INHERITABLE: the parameter can be inherited from an ancestor
 */
typedef enum /*< skip >*/
{
    DAX_PARAM_NONE           = 0,
    DAX_PARAM_VERSION_1_0    = 1 << 0,
    DAX_PARAM_VERSION_1_1    = 1 << 1,
    DAX_PARAM_VERSION_1_2    = 1 << 2,
    DAX_PARAM_PROFILE_FULL   = 1 << 3,
    DAX_PARAM_PROFILE_MOBILE = 1 << 4,
    DAX_PARAM_PROFILE_TINY   = 1 << 5,
    DAX_PARAM_PROPERTY       = 1 << 6,
    DAX_PARAM_ANIMATABLE     = 1 << 7,
    DAX_PARAM_INHERITABLE    = 1 << 8
} DaxParamFlags;

/*
 * DaxParamSpecEnum
 */

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

GType           dax_param_enum_get_type     (void) G_GNUC_CONST;
GParamSpec *    dax_param_spec_enum         (const gchar   *name,
                                             const gchar   *nick,
                                             const gchar   *blurb,
                                             GType          enum_type,
                                             gint           default_value,
                                             GParamFlags    g_flags,
                                             DaxParamFlags  dax_flags,
                                             const gchar   *namespace_uri);

/*
 * DaxParamSpecString
 */

/**
 * DaxParamSpecString:
 *
 * A #GParamSpecString derived structure that contains extra meta data for string
 * properties.
 */
struct _DaxParamSpecString
{
  GParamSpecString  parent_instance;

  DaxParamFlags   flags;
  const gchar    *namespace_uri;
};

/**
 * DAX_TYPE_PARAM_STRING:
 *
 * The #GType of #DaxParamSpecString.
 */
#define DAX_TYPE_PARAM_STRING          (dax_param_string_get_type ())

/**
 * DAX_IS_PARAM_SPEC_STRING:
 * @pspec: a valid #GParamSpec instance
 *
 * Checks whether the given #GParamSpec is of type %DAX_TYPE_PARAM_STRING.
 *
 * Returns: %TRUE on success.
 */
#define DAX_IS_PARAM_SPEC_STRING(pspec) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), DAX_TYPE_PARAM_STRING))

/**
 * DAX_PARAM_SPEC_STRING:
 * @pspec: a valid #GParamSpec instance
 *
 * Cast a #GParamSpec instance into a #DaxParamSpecString.
 */
#define DAX_PARAM_SPEC_STRING(pspec)                            \
          (G_TYPE_CHECK_INSTANCE_CAST ((pspec),                 \
                                       DAX_TYPE_PARAM_STRING,   \
                                       DaxParamSpecString))

GType           dax_param_string_get_type     (void) G_GNUC_CONST;
GParamSpec *    dax_param_spec_string         (const gchar   *name,
                                               const gchar   *nick,
                                               const gchar   *blurb,
                                               const gchar   *default_value,
                                               GParamFlags    g_flags,
                                               DaxParamFlags  dax_flags,
                                               const gchar   *namespace_uri);

/*
 * DaxParamSpecBoxed
 */

/**
 * DaxParamSpecBoxed:
 *
 * A #GParamSpecBoxed derived structure that contains extra meta data for boxed
 * properties.
 */
struct _DaxParamSpecBoxed
{
  GParamSpecBoxed  parent_instance;

  DaxParamFlags    flags;
  const gchar     *namespace_uri;
};

/**
 * DAX_TYPE_PARAM_BOXED:
 *
 * The #GType of #DaxParamSpecBoxed.
 */
#define DAX_TYPE_PARAM_BOXED          (dax_param_boxed_get_type ())

/**
 * DAX_IS_PARAM_SPEC_BOXED:
 * @pspec: a valid #GParamSpec instance
 *
 * Checks whether the given #GParamSpec is of type %DAX_TYPE_PARAM_BOXED.
 *
 * Returns: %TRUE on success.
 */
#define DAX_IS_PARAM_SPEC_BOXED(pspec) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), DAX_TYPE_PARAM_BOXED))

/**
 * DAX_PARAM_SPEC_BOXED:
 * @pspec: a valid #GParamSpec instance
 *
 * Cast a #GParamSpec instance into a #DaxParamSpecBoxed.
 */
#define DAX_PARAM_SPEC_BOXED(pspec)                             \
          (G_TYPE_CHECK_INSTANCE_CAST ((pspec),                 \
                                       DAX_TYPE_PARAM_BOXED,    \
                                       DaxParamSpecBoxed))

GType           dax_param_boxed_get_type     (void) G_GNUC_CONST;
GParamSpec *    dax_param_spec_boxed         (const gchar   *name,
                                              const gchar   *nick,
                                              const gchar   *blurb,
                                              GType          boxed_type,
                                              GParamFlags    g_flags,
                                              DaxParamFlags  dax_flags,
                                              const gchar   *namespace_uri);

/*
 * DaxParamSpecArray
 */

/**
 * DaxParamSpecArray:
 *
 * A #GParamSpecArray derived structure that contains extra meta data for array
 * properties.
 */
struct _DaxParamSpecArray
{
  GParamSpecBoxed  parent_instance;

  GType            element_type;
  gint             size;
  DaxParamFlags    flags;
  const gchar     *namespace_uri;
};

/**
 * DAX_TYPE_PARAM_ARRAY:
 *
 * The #GType of #DaxParamSpecArray.
 */
#define DAX_TYPE_PARAM_ARRAY          (dax_param_array_get_type ())

/**
 * DAX_IS_PARAM_SPEC_ARRAY:
 * @pspec: a valid #GParamSpec instance
 *
 * Checks whether the given #GParamSpec is of type %DAX_TYPE_PARAM_ARRAY.
 *
 * Returns: %TRUE on success.
 */
#define DAX_IS_PARAM_SPEC_ARRAY(pspec) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), DAX_TYPE_PARAM_ARRAY))

/**
 * DAX_PARAM_SPEC_ARRAY:
 * @pspec: a valid #GParamSpec instance
 *
 * Cast a #GParamSpec instance into a #DaxParamSpecArray.
 */
#define DAX_PARAM_SPEC_ARRAY(pspec)                             \
          (G_TYPE_CHECK_INSTANCE_CAST ((pspec),                 \
                                       DAX_TYPE_PARAM_ARRAY,    \
                                       DaxParamSpecArray))

#define DAX_PARAM_SPEC_ARRAY_NOT_SIZED  (-1)

GType           dax_param_array_get_type     (void) G_GNUC_CONST;
GParamSpec *    dax_param_spec_array         (const gchar   *name,
                                              const gchar   *nick,
                                              const gchar   *blurb,
                                              GType          element_type,
                                              gint           size,
                                              GParamFlags    g_flags,
                                              DaxParamFlags  dax_flags,
                                              const char    *namespace_uri);

G_END_DECLS

#endif /* __DAX_PARAMSPEC_H__ */
