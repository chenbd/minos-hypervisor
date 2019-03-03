/*
 * Copyright (C) 2018 Min Le (lemin9538@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <minos/minos.h>
#include <minos/sched.h>


static struct sched_class *sched_classes[MAX_SCHED_CLASS];

int register_sched_class(struct sched_class *cls)
{
	struct sched_class *c;

	if ((!cls) || (cls->name >= MAX_SCHED_CLASS))
		return -EINVAL;

	c = sched_classes[cls->name];
	if (c)
		return -EEXIST;

	sched_classes[cls->name] = cls;
	return 0;
}

struct sched_class *get_sched_class(int32_t name)
{
	if (name >= MAX_SCHED_CLASS)
		return NULL;
	return sched_classes[name];
}
