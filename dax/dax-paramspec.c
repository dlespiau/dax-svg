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

#include "dax-internals.h"
#include "dax-paramspec.h"

/*
 * Custom GParamSpec
 *
 * Dax needs custom GParamSpec to add a few features needed for SVG, namely:
 *   * differenciation between properties and attributes,
 *   * handling of inheritance,
 *   * specify if the parameter is animatable.
 */

static void
dax_param_enum_init (GParamSpec *pspec)
{
#if 0
    DaxParamSpecEnum *espec = DAX_PARAM_SPEC_ENUM (pspec);
#endif
}

static void
dax_param_enum_class_init (GParamSpecClass *klass)
{
    klass->value_type = G_TYPE_ENUM;
}

GType
dax_param_enum_get_type (void)
{
    static volatile gsize dax_param_enum_type__volatile = 0;

    if (g_once_init_enter (&dax_param_enum_type__volatile)) {
        GType type;

        static const GTypeInfo info = {
            sizeof (GParamSpecClass),
            NULL,
            NULL,
            (GClassInitFunc) dax_param_enum_class_init,
            NULL,
            NULL,
            sizeof (DaxParamSpecEnum),
            0,
            (GInstanceInitFunc) dax_param_enum_init
        };

        type = g_type_register_static (G_TYPE_PARAM_ENUM,
                                       "DaxParamSpecEnum",
                                       &info,
                                       0);

        g_once_init_leave (&dax_param_enum_type__volatile, type);
    }

    return dax_param_enum_type__volatile;
}

/**
 * dax_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @enum_type: a #GType derived from %G_TYPE_ENUM
 * @default_value: default value for the property specified
 * @g_flags: flags for the underlying GParamSpec property
 * @dax_flags: flags specific to Dax
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #GParamSpecEnum instance specifying a %G_TYPE_ENUM
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
dax_param_spec_enum (const gchar   *name,
                     const gchar   *nick,
                     const gchar   *blurb,
                     GType          enum_type,
                     gint           default_value,
                     GParamFlags    g_flags,
                     DaxParamFlags  dax_flags,
                     const char    *namespace_uri)
{
    DaxParamSpecEnum *dax_enum_spec;
    GParamSpecEnum *g_enum_spec;
    GEnumClass *enum_class;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref (enum_type);

    g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL,
                          NULL);

    dax_enum_spec = g_param_spec_internal (DAX_TYPE_PARAM_ENUM,
                                           name,
                                           nick,
                                           blurb,
                                           g_flags);

    G_PARAM_SPEC (dax_enum_spec)->value_type = enum_type;

    g_enum_spec = G_PARAM_SPEC_ENUM (dax_enum_spec);
    g_enum_spec->enum_class = enum_class;
    g_enum_spec->default_value = default_value;

    dax_enum_spec->flags = dax_flags;
    dax_enum_spec->namespace_uri = namespace_uri;

    return G_PARAM_SPEC (dax_enum_spec);
}
