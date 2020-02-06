// SPDX-License-Identifier: GPL-2.0-only
//
// memd
//
// Copyright (c) 20?? Hiroyuki Yokoyama <hiroyuki.yokoyama.vx@renesas.com>
// Copyright (c) 2019 Yuya Hamamachi <yuya.hamamachi.sx@renesas.com>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include "../version.h"

#define PROCNAME "reg"
#define RECV_BUFF_SIZE 256
#define STR_SIZE 64
#define FMT_SIZE 32
#define MAX_COMMAND_LENGTH 2

enum cmd_name {
	D_MEM_READ,
	D_MEM_WRITE,
	D_MEM_DUMP,
	D_MEM_NUM,
};

static char cmd_name[] = {
	[D_MEM_READ]  = 'r',	/* memory read   8bit */
	[D_MEM_WRITE] = 'w',	/* memory write  8bit */
	[D_MEM_DUMP]  = 'm',	/* memory dump */
};

struct cmd_param {
	int cmd;
	void __iomem *reg;
	unsigned long addr;
	unsigned long val;
	int access_size;
	unsigned long mapping_size;
	struct memd_private_data *pdata;
};

struct memd_private_data {
	struct mutex mutex; /* for read/write/dump */
	long read_val;
	int read_val_len;
};

static int buff_parser(const char __user *buffer, unsigned long buffer_len,
		       struct cmd_param *prm)
{
	char buff[RECV_BUFF_SIZE];
	char str[STR_SIZE];
	char fmt[FMT_SIZE];
	int arguments_num;
	int i;

	if (buffer_len >= RECV_BUFF_SIZE)
		return -EINVAL;

	if (copy_from_user(buff, buffer, buffer_len))
		return -EFAULT;

	buff[buffer_len] = '\0';
	pr_debug("Recv Command : [%s] (len = %ld)\n", buff, buffer_len);

	sprintf(fmt, "%%%ds %%lx %%lx", STR_SIZE-1);
	arguments_num = sscanf(buff, fmt, str, &prm->addr, &prm->val);
	pr_debug("  cmd : [%s]\n  addr: [%lx]\n  val : [%lx]\n",
		 str, prm->addr, prm->val);

	/* get command ID */
	for (i = 0; i < D_MEM_NUM; i++) {
		if (str[0] == cmd_name[i]) {
			prm->cmd = i;
			break;
		}
	}
	if (i >= D_MEM_NUM)
		return -EINVAL;

	/* get access_size */
	switch (str[1]) {
	case '\0':
		prm->access_size = 1;
		break;
	case 'w':
		prm->access_size = 2;
		break;
	case 'd':
		prm->access_size = 4;
		break;
	default:
		return -EINVAL;
	}

	/* check command length */
	if (strlen(str) > MAX_COMMAND_LENGTH)
		return -EINVAL;

	/* get mapping size */
	switch (prm->cmd) {
	case D_MEM_READ:
		if (arguments_num < 2)
			return -EINVAL;
		prm->mapping_size = prm->access_size;
		break;
	case D_MEM_WRITE:
		if (arguments_num < 3)
			return -EINVAL;
		prm->mapping_size = prm->access_size;
		break;
	case D_MEM_DUMP:
		if (arguments_num < 2)
			return -EINVAL;
		prm->mapping_size = (arguments_num < 3) ? 0x80 : prm->val;
		break;
	}

	/* check alignment */
	if (prm->addr % prm->access_size)
		return -EINVAL;

	return 0;
}

int mem_read(const struct cmd_param *prm)
{
	long val = -1;

	switch (prm->access_size) {
	case 1:
		val = readb(prm->reg);
		break;
	case 2:
		val = readw(prm->reg);
		break;
	case 4:
		val = readl(prm->reg);
		break;
	}
	pr_info("  mem read [%08lX] : %0*lX\n",
		prm->addr, prm->access_size*2, val);

	prm->pdata->read_val = val;
	prm->pdata->read_val_len = prm->access_size * 2;

	return 0;
}

int mem_write(const struct cmd_param *prm)
{
	switch (prm->access_size) {
	case 1:
		writeb(prm->val, prm->reg);
		break;
	case 2:
		writew(prm->val, prm->reg);
		break;
	case 4:
		writel(prm->val, prm->reg);
		break;
	}
	pr_debug("  mem write [%08lX] : %0*lX\n",
		 prm->addr, prm->access_size*2, prm->val);

	return 0;
}

int mem_dump(const struct cmd_param *prm)
{
	struct cmd_param tmp;
	int i;
	long val = -1;

	/*
	 * mem_dump() needs to update addr/reg,
	 * and we need to use const *prm.
	 * see
	 *	memd_proc_write() :: iounmap()
	 *
	 * Here uses copied cmd_param
	 */
	tmp = *prm;
	for (i = 0; i < tmp.mapping_size; i += tmp.access_size) {
		switch (tmp.access_size) {
		case 1:
			val = readb(tmp.reg);
			break;
		case 2:
			val = readw(tmp.reg);
			break;
		case 4:
			val = readl(tmp.reg);
			break;
		}
		pr_info("  mem read [%08lX] : %0*lX\n",
			tmp.addr, tmp.access_size*2, val);

		tmp.addr += tmp.access_size;
		tmp.reg  += tmp.access_size;
	}

	return 0;
}

ssize_t memd_proc_write(struct file *file, const char __user *buffer,
			size_t len, loff_t *pos)
{
	int ret;
	struct cmd_param prm;
	struct memd_private_data *pdata = file->private_data;

	prm.pdata = pdata;
	ret = buff_parser(buffer, len, &prm);
	if (ret)
		return ret;

	prm.reg = ioremap_nocache(prm.addr, prm.mapping_size);
	if (!prm.reg) {
		pr_err("  ioremap fail [%08lX]\n", prm.addr);
		return -ENOMEM;
	}

	/* start critical section */
	mutex_lock(&pdata->mutex);

	switch (prm.cmd) {
	case D_MEM_READ:
		ret = mem_read(&prm);
		break;
	case D_MEM_DUMP:
		ret = mem_dump(&prm);
		break;
	case D_MEM_WRITE:
		ret = mem_write(&prm);
		break;
	default:
		pr_err("  Fatal error\n");
		ret = -EINVAL;
	}

	mutex_unlock(&pdata->mutex);
	/* end critical section */

	iounmap(prm.reg);

	return ret ? : len;
}

static ssize_t memd_proc_read(struct file *file, char __user *buff, size_t len,
			      loff_t *pos)
{
	int ret = 0;
	struct memd_private_data *pdata = file->private_data;
	char msg_buff[2 * sizeof(long) + 1];

	/* start critical section */
	mutex_lock(&pdata->mutex);

	if (!pdata->read_val_len)
		goto UNLOCK;

	sprintf(msg_buff, "%0*lx", pdata->read_val_len, pdata->read_val);
	if (copy_to_user(buff, msg_buff, pdata->read_val_len)) {
		ret = -EFAULT;
		goto UNLOCK;
	}
	ret = pdata->read_val_len;
	pdata->read_val_len = 0;

UNLOCK:
	mutex_unlock(&pdata->mutex);
	/* end critical section */

	return ret;
}

static int memd_proc_open(struct inode *inode, struct file *file)
{
	struct memd_private_data *pdata = PDE_DATA(inode);

	file->private_data = pdata;

	return 0;
}

static const struct file_operations entry_proc_fops = {
	.owner = THIS_MODULE,
	.write = memd_proc_write,
	.read  = memd_proc_read,
	.open  = memd_proc_open,
};

static int memd_init(void)
{
	static struct memd_private_data pdata;

	mutex_init(&pdata.mutex);
	pdata.read_val_len = 0;
	proc_create_data(PROCNAME, 0600, NULL, &entry_proc_fops, &pdata);
	pr_info("memd driver loaded (%s)\n", version);

	return 0;
}

static void memd_exit(void)
{
	remove_proc_entry(PROCNAME, NULL);
	pr_info("driver unloaded\n");
}

module_init(memd_init);
module_exit(memd_exit);

MODULE_LICENSE("GPL v2");
