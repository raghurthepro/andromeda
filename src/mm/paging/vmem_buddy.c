/*
 * Andromeda
 * Copyright (C) 2011  Bart Kuivenhoven
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * \AddToGroup vmem
 * @{
 */
#include <mm/vmem.h>
#include <mm/paging.h>
#include <andromeda/error.h>

/**
 * \todo Build a buddy allocator initialiser
 * \todo Build a buddy region splitter
 * \todo Build a buddy region merger
 * \todo Build the buddy allocator
 * \todo Build the buddy freeing function
 */

struct vmem_buddy_system*
vmem_buddy_system_init()
{
        return NULL;
}

int
vmem_buddy_system_reset(struct vmem_buddy_system* system)
{
        return -E_NOFUNCTION;
}

int
vmem_buddy_purge(struct vmem_buddy* buddy, idx_t list)
{
        return -E_NOFUNCTION;
}

int
vmem_buddy_set(struct vmem_buddy* buddy, idx_t list)
{
        return -E_NOFUNCTION;
}

struct vmem_buddy*
find_adjecent(struct vmem_buddy* buddy)
{
}

struct vmem_buddy*
vmem_buddy_split(struct vmem_buddy* buddy)
{
        if (buddy == NULL)
                return NULL;

        struct vmem_buddy* new = kalloc(sizeof(*new));
        if (new == NULL)
                return NULL;
        memset(new, 0, sizeof(*new));

        buddy->size /= 2;
        new->size = buddy->size;
        new->ptr = buddy->ptr + buddy->size;
        new->prev = buddy;
        new->next = buddy->next;
        new->system = buddy->system;
        buddy->next = new;
        vmem_buddy_purge(buddy, log2i(buddy->size)+1);
        vmem_buddy_purge(new, log2i(new->size)+1);
        vmem_buddy_set(buddy, log2i(buddy->size));
        vmem_buddy_set(new, log2i(new->size));
}

int
vmem_buddy_merge(struct vmem_buddy* a, struct vmem_buddy* b)
{
        return -E_NOFUNCTION;
}

void*
vmem_buddy_alloc(struct vmem_buddy_system* system, size_t size)
{
        return NULL;
}

void
vmem_buddy_free(struct vmem_buddy_system* system, void* ptr)
{
}

/**
 * @}
 * \file
 */
