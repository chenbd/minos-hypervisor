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
#include <minos/vmodule.h>
#include <minos/init.h>
#include <minos/mm.h>
#include <minos/vcpu.h>
#include <minos/spinlock.h>

extern unsigned char __vmodule_start;
extern unsigned char __vmodule_end;

static int vmodule_class_nr = 0;
static LIST_HEAD(vmodule_list);

static struct vmodule *create_vmodule(struct module_id *id)
{
	struct vmodule *vmodule;
	vmodule_init_fn fn;

	vmodule = (struct vmodule *)
		malloc(sizeof(struct vmodule));
	if (!vmodule) {
		pr_error("No more memory for vmodule\n");
		return NULL;
	}

	memset((char *)vmodule, 0, sizeof(struct vmodule));
	strncpy(vmodule->name, id->name, 31);
	init_list(&vmodule->list);
	vmodule->id = vmodule_class_nr;
	vmodule_class_nr++;

	/* call init routine */
	if (id->data) {
		fn = (vmodule_init_fn)id->data;
		fn(vmodule);
	}

	list_add(&vmodule_list, &vmodule->list);
	return vmodule;
}

int register_vcpu_vmodule(char *name, vmodule_init_fn fn)
{
	struct module_id mid;
	struct vmodule *vmodule;

	memset(&mid, 0, sizeof(struct module_id));
	mid.data = fn;
	mid.comp = NULL;
	strncpy(mid.name, name, strlen(name) > 31 ? 31 : strlen(name));

	vmodule = create_vmodule(&mid);
	if (!vmodule)
		pr_error("create vmodule %s failed\n", name);

	return 0;
}

void *get_vmodule_data_by_id(struct vcpu *vcpu, int id)
{
	return vcpu->vmodule_context[id];
}

void *get_vmodule_data_by_name(struct vcpu *vcpu, char *name)
{
	struct vmodule *vmodule;
	int id = INVAILD_MODULE_ID;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		if (strcmp(vmodule->name, name) == 0)
			id = vmodule->id;
	}

	if (id != INVAILD_MODULE_ID)
		return vcpu->vmodule_context[id];

	return NULL;
}

int vcpu_vmodules_init(struct vcpu *vcpu)
{
	struct list_head *list;
	struct vmodule *vmodule;
	void *data;
	int size;

	/*
	 * firset allocate memory to store each vmodule
	 * context's context data
	 */
	size = vmodule_class_nr * sizeof(void *);
	vcpu->vmodule_context = (void **)malloc(size);
	if (!vcpu->vmodule_context)
		panic("No more memory for vcpu vmodule cotnext\n");

	memset((char *)vcpu->vmodule_context, 0, size);

	list_for_each(&vmodule_list, list) {
		vmodule = list_entry(list, struct vmodule, list);
		if (vmodule->context_size) {
			/* for reboot if memory is areadly allocated skip it */
			data = vcpu->vmodule_context[vmodule->id];
			if (!data) {
				data = (void *)malloc(vmodule->context_size);
				vcpu->vmodule_context[vmodule->id] = data;
			}

			memset((char *)data, 0, vmodule->context_size);
			if (vmodule->state_init)
				vmodule->state_init(vcpu, data);
		}
	}

	return 0;
}

int vcpu_vmodules_deinit(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *data;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		data = vcpu->vmodule_context[vmodule->id];
		if (vmodule->state_deinit)
			vmodule->state_deinit(vcpu, data);

		if (data)
			free(data);
	}

	return 0;
}

int vcpu_vmodules_reset(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *data;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		data = vcpu->vmodule_context[vmodule->id];
		if (vmodule->state_reset)
			vmodule->state_reset(vcpu, data);
	}

	return 0;
}

void restore_vcpu_vmodule_state(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *context;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		if (vmodule->state_restore) {
			context = get_vmodule_data_by_id(vcpu, vmodule->id);
			vmodule->state_restore(vcpu, context);
		}
	}
}

void save_vcpu_vmodule_state(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *context;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		if (vmodule->state_save) {
			context = get_vmodule_data_by_id(vcpu, vmodule->id);
			vmodule->state_save(vcpu, context);
		}
	}
}

void suspend_vcpu_vmodule_state(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *context;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		if (vmodule->state_suspend) {
			context = get_vmodule_data_by_id(vcpu, vmodule->id);
			vmodule->state_suspend(vcpu, context);
		}
	}
}

void resume_vcpu_vmodule_state(struct vcpu *vcpu)
{
	struct vmodule *vmodule;
	void *context;

	list_for_each_entry(vmodule, &vmodule_list, list) {
		if (vmodule->state_resume) {
			context = get_vmodule_data_by_id(vcpu, vmodule->id);
			vmodule->state_resume(vcpu, context);
		}
	}
}

int vmodules_init(void)
{
	int32_t i;
	unsigned long base, end;
	uint32_t size;
	struct module_id *mid;
	struct vmodule *vmodule;

	base = (unsigned long)&__vmodule_start;
	end = (unsigned long)&__vmodule_end;
	size = (end - base) / sizeof(struct module_id);

	for (i = 0; i < size; i++) {
		mid = (struct module_id *)base;
		vmodule = create_vmodule(mid);
		if (!vmodule)
			pr_error("Can not create vmodule\n");

		base += sizeof(struct module_id);
	}

	return 0;
}
