/*
 *  Andromeda, The educational operatingsystem
 *  Copyright (C) 2011  Bart Kuivenhoven
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <andromeda/drivers.h>
#include <drivers/root.h>

struct device dev_root;
uint64_t dev_id = 0;
mutex_t dev_id_lock;

uint64_t virt_bus = 0;
uint64_t lgcy_bus = 0;

/** Return the number of detected CPU's */
int dev_detect_cpus(struct device* root)
{
        return -E_NOFUNCTION;
}

int dev_root_init()
{
        struct device* root = &dev_root;
        memset(root, 0, sizeof(struct device));
        root->driver = kalloc(sizeof(struct driver));
        memset(root->driver, 0, sizeof(struct driver));

        drv_root_init(root); /** Call to driver, not device!!! */

//         if (dev_detect_cpus(root) <= 0)
//         {
//                 panic("Couldn't find any CPU's!");
//         }

        return -E_SUCCESS;
}

int device_recurse_suspend(struct device* this)
{
        struct device* cariage = this->children;

        while(cariage != NULL)
        {
                cariage->driver->suspend(cariage);
                cariage = cariage->next;
        }
        return -E_SUCCESS;
}

int device_recurse_resume(struct device* this)
{
        struct device* cariage = this->children;

        while(cariage != NULL)
        {
                cariage->driver->resume(cariage);
                cariage = cariage->next;
        }
        return -E_SUCCESS;
}

int device_attach(struct device* this, struct device* child)
{
        if (this->children == NULL)
        {
                this->children = child;
                return -E_SUCCESS;
        }
        struct device* cariage = this->children;
        struct device* last = cariage;
        while (cariage != NULL)
        {
                last = cariage;
                cariage = cariage->next;
        }
        last->next = child;
        return -E_SUCCESS;
}

int device_detach(struct device* this, struct device* child)
{
        struct device* cariage = this->children;
        struct device* last = cariage;
        while (cariage != NULL)
        {
                if (cariage == child)
                {
                        if (cariage == last)
                        {
                                this->children = cariage->next;
                        }
                        else
                        {
                                last->next = cariage->next;
                                cariage->next = NULL;
                        }
                }
        }
}

struct device*
device_find_id(struct device* this, uint64_t dev_id)
{
        if (this == NULL)
                return NULL;

        if (dev_id == this->dev_id)
                return this;

        struct device* cariage = this->children;

        struct device *dev;
        while (cariage != NULL)
        {
                if (cariage->driver == NULL)
                        panic("Driverless attached device!");
                if (cariage->driver->find == NULL)
                        panic("No find function in device!");

                dev = cariage->driver->find(cariage, dev_id);
                if (dev != NULL)
                        return dev;

                cariage = cariage->next;
        }
        return NULL;
}

int device_id_alloc(struct device* dev)
{
        int ret = 0;
        uint64_t begin = dev_id-1;
        mutex_lock(dev_id_lock);
        struct device* iterator = device_find_id(&dev_root, dev_id);
        while (iterator != NULL)
        {
                dev_id++;
                if (dev_id == begin)
                        panic("No more room for new devices!");
                iterator = device_find_id(&dev_root, dev_id);
        }

        dev->dev_id = dev_id;
        ret = dev_id;

        mutex_unlock(dev_id_lock);
        return ret;
}

void dev_dbg()
{
        int i = 0;
        for (; i < 0x10; i++)
        {
                printf("Device 0x%X at address %X\n", i, device_find_id(&dev_root, i));
        }
}

int
dev_init()
{
        debug("Building the device tree\n");

        dev_root_init();

#ifdef DEV_DBG
        dev_dbg();
#endif

        return -E_SUCCESS;
}