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
	[D_MEM_READ]    = 'r',	/* memory read   8bit   */
	[D_MEM_WRITE]   = 'w',	/* memory write  8bit   */
	[D_MEM_DUMP]    = 'm',	/* memory dump */
};

struct cmd_param {
	int cmd;
	void __iomem *reg;
	unsigned long addr;
	unsigned long val;
	int access_size;
	unsigned long mapping_size;
};

static int buff_parser(char *buff, struct cmd_param *prm)
{
	char str[STR_SIZE];
	char fmt[FMT_SIZE];
	int arguments_num;
	int i;

	sprintf(fmt, "%%%ds %%lx %%lx", STR_SIZE-1);
	arguments_num = sscanf(buff, fmt, str, &prm->addr, &prm->val);
	pr_debug("  cmd : [%s]\n  addr: [%lx]\n  val : [%lx]\n", str, prm->addr,
								prm->val);

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
		prm->mapping_size = 0x80;
		break;
	}

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
	pr_info("  mem read [%08lX] : %0*lX\n", prm->addr, prm->access_size*2,
						val);

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
	pr_debug("  mem write [%08lX] : %0*lX\n", prm->addr, prm->access_size*2,
						  prm->val);

	return 0;
}


int mem_dump(const struct cmd_param *prm)
{
	long val;
	int i;

	if (prm->access_size != 4) {
		pr_warn("  md is used instead of m/mw.\n");
		pr_warn("  %s supports 4 bytes access only, for now.\n",
								      __func__);
	}

	for (i = 0; i < prm->mapping_size/4; i += 4) {
		val = readl(prm->reg + i);
		pr_info("  mem read [%08lX] : %08lX\n", prm->addr + i, val);
	}

	return 0;
}

ssize_t memd_proc_write(struct file *file, const char __user *buffer,
			size_t count, loff_t *pos)
{
	char buff[RECV_BUFF_SIZE];
	unsigned long len = count;
	int ret;
	struct cmd_param prm;

	if (len >= RECV_BUFF_SIZE)
		return -EINVAL;
	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	buff[len] = '\0';

	pr_debug("Recv Command : [%s] (len = %ld)\n", buff, len);

	ret = buff_parser(buff, &prm);
	if (ret)
		return ret;

	prm.reg = ioremap_nocache(prm.addr, prm.mapping_size);
	if (!prm.reg) {
		pr_err("  ioremap fail [%08lX]\n", prm.addr);
		return -ENOMEM;
	}

	switch (prm.cmd) {
	case D_MEM_READ:
		ret = mem_read(&prm);
		break;
	case D_MEM_WRITE:
		ret = mem_write(&prm);
		break;
	case D_MEM_DUMP:
		ret = mem_dump(&prm);
		break;
	default:
		pr_err("  Fatal error\n");
		ret = -EINVAL;
	}

	iounmap(prm.reg);

	return ret ? : len;
}


static const struct file_operations entry_proc_fops = {
	.owner = THIS_MODULE,
	.write  = memd_proc_write,
};

static int memd_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create_data(PROCNAME, 0600, NULL, &entry_proc_fops, NULL);
	pr_info("memd driver loaded\n");

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
