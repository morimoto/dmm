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

static int buff_parser(char *buff, int *cmd, unsigned long *addr,
		       unsigned long *val, int *access_size)
{
	char str[STR_SIZE];
	char fmt[FMT_SIZE];
	int arguments_num;
	int i;

	sprintf(fmt, "%%%ds %%lx %%lx", STR_SIZE-1);
	arguments_num = sscanf(buff, fmt, str, addr, val);
	pr_debug("  cmd : [%s]\n  addr: [%lx]\n  val : [%lx]\n", str, *addr,
								*val);

	/* get command ID */
	for (i = 0; i < D_MEM_NUM; i++) {
		if (str[0] == cmd_name[i]) {
			*cmd = i;
			break;
		}
	}
	if (i >= D_MEM_NUM)
		return -EINVAL;

	/* get access_size */
	switch (str[1]) {
	case '\0':
		*access_size = 1;
		break;
	case 'w':
		*access_size = 2;
		break;
	case 'd':
		*access_size = 4;
		break;
	default:
		return -EINVAL;
	}

	/* check command length */
	if (strlen(str) > MAX_COMMAND_LENGTH)
		return -EINVAL;

	/* check argument */
	switch (*cmd) {
	case D_MEM_READ:
	case D_MEM_DUMP:
		if (arguments_num < 2)
			return -EINVAL;
		break;
	case D_MEM_WRITE:
		if (arguments_num < 3)
			return -EINVAL;
		break;
	}

	return 0;
}



int mem_read(unsigned long addr, int size)
{
	void __iomem *reg;
	long val = -1;

	reg = ioremap_nocache(addr, size);
	if (!reg) {
		pr_err("  ioremap fail [%08lX]\n", addr);
		return -ENOMEM;
	}

	switch (size) {
	case 1:
		val = readb(reg);
		break;
	case 2:
		val = readw(reg);
		break;
	case 4:
		val = readl(reg);
		break;
	}
	pr_info("  mem read [%08lX] : %0*lX\n", addr, size*2, val);

	iounmap(reg);

	return 0;
}

int mem_write(unsigned long addr, unsigned long val, int size)
{
	void __iomem *reg;

	reg = ioremap_nocache(addr, size);
	if (!reg) {
		pr_err("  ioremap fail [%08lX]\n", addr);
		return -ENOMEM;
	}

	switch (size) {
	case 1:
		writeb(val, reg);
		break;
	case 2:
		writew(val, reg);
		break;
	case 4:
		writel(val, reg);
		break;
	}
	pr_debug("  mem write [%08lX] : %0*lX\n", addr, size*2, val);

	iounmap(reg);

	return 0;
}


int mem_dump(unsigned long addr, int access_size, int size)
{
	void __iomem  *reg;
	long val;
	int i;

	reg = ioremap_nocache(addr, size);
	if (!reg) {
		pr_err("  ioremap fail [%08lX]\n", addr);
		return -ENOMEM;
	}

	if (access_size != 4) {
		pr_warn("  md is used instead of m/mw.\n");
		pr_warn("  %s supports 4 bytes access only, for now.\n",
								      __func__);
	}

	for (i = 0; i < size/4; i += 4 ) {
		val = readl(reg + i);
		pr_info("  mem read [%08lX] : %08lX\n", addr + i, val);
	}

	iounmap(reg);

	return 0;
}

ssize_t memd_proc_write(struct file *file, const char __user *buffer,
			size_t count, loff_t *pos)
{
	char buff[RECV_BUFF_SIZE];
	unsigned long len = count;
	int ret;
	int cmd;
	unsigned long addr;
	unsigned long val;
	int access_size;

	if (len >= RECV_BUFF_SIZE)
		return -EINVAL;
	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	buff[len] = '\0';

	pr_debug("Recv Command : [%s] (len = %ld)\n", buff, len);

	ret = buff_parser(buff, &cmd, &addr, &val, &access_size);
	if (ret)
		return ret;

	switch (cmd) {
	case D_MEM_READ:
		ret = mem_read(addr, access_size);
		break;
	case D_MEM_WRITE:
		ret = mem_write(addr, val, access_size);
		break;
	case D_MEM_DUMP:
		ret = mem_dump(addr, access_size, 0x80);
		break;
	default:
		pr_err("  Fatal error\n");
		ret = -EINVAL;
	}

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
