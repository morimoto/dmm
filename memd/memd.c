#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/uaccess.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");

#if 0
#define debug
#endif

#define PROCNAME "reg"
#define NUM_CMD 7

static char *cmd_name[] = {
	"r"  ,	/* memory read   8bit   */
	"rw" ,	/*              16bit   */
	"rd" ,	/*              32bit   */
	"w"  ,	/* memory write  8bit   */
	"ww" ,	/*              16bit   */
	"wd" ,	/*              32bit   */
	"md" ,  /* memory dump */
};

enum cmd_name {
	D_MEM_READ,
	D_MEM_READ_W,
	D_MEM_READ_D,
	D_MEM_WRITE,
	D_MEM_WRITE_W,
	D_MEM_WRITE_D,
	D_MEM_DUMP,
};

static int search_cmd(char *str, int *cmd)
{
	int i;
	int ret = -1;

	for (i = 0; str[i] != '\0'; i++)
		str[i] = tolower(str[i]);

	for (i = 0; i < NUM_CMD; i++) {
		if (!strcmp(str, cmd_name[i])) {
			*cmd = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

static int cut_buff(char *buff, int *cmd, unsigned long *addr, unsigned long *val)
{
	char str[64];
	int i;
	int ret;

	/* get command name */
	i = 0;
	while (*buff != ' ')
		str[i++] = *buff++;

	str[i] = '\0';
#ifdef debug
	pr_info("  cmd : [%s] ", str);
#endif

	while (*buff == ' ')
		buff++;

	if (search_cmd(str, cmd))
		return -EINVAL;

#ifdef debug
	pr_info("(%02X)\n", *cmd);
#endif

	/* get addr */
	i = 0;
	while (*buff != ' ' && *buff != '\0')
		str[i++] = *buff++;

	str[i] = '\0';

#ifdef debug
	pr_info("  addr : [%s] ", str);
#endif

	ret = kstrtoul(str, 0, addr);
	if (ret) {
		pr_info("error(%d)  Invalid addr [%s]\n", ret, str);
		return -1;
	}

	while (*buff == ' ')
		buff++;

#ifdef debug
	pr_info("(%lX)\n", *addr);
#endif

	/* get Value */
	if (*cmd == D_MEM_WRITE || *cmd == D_MEM_WRITE_W ||
			*cmd == D_MEM_WRITE_D) {
		i = 0;
		while (*buff != '\0')
			str[i++] = *buff++;

		str[i] = '\0';

#ifdef debug
		pr_info("val : %s(", str);
#endif
		ret = kstrtoul(str, 0, val);
		if (ret) {
			pr_info("error(%d)  Invalid value [%s]\n", ret, str);
			return -1;
		}
#ifdef debug
		pr_info("%lX)\n", *val);
#endif
	}

	return 0;
}



int mem_read(unsigned long addr, int size)
{
	char *reg;
	long val;

	reg = (char *)ioremap_nocache(addr, size);

	if (reg != NULL) {

		switch (size) {
		case 1:
			val = readb(reg);
			pr_info("  mem read [%08lX] : %02lX\n", addr, val);
			break;
		case 2:
			val = readw(reg);
			pr_info("  mem read [%08lX] : %04lX\n", addr, val);
			break;
		case 4:
			val = readl(reg);
			pr_info("  mem read [%08lX] : %08lX\n", addr, val);
			break;
		}
		iounmap(reg);
	} else {
		pr_info("  ioremap fail [%08lX]\n", addr);
		return -1;
	}

	return 0;
}



int mem_write(unsigned long addr, unsigned long val, int size)
{
	char *reg;

	reg = ioremap_nocache(addr, size);

	if (reg != NULL) {
		switch (size) {
		case 1:
			writeb(val, reg);
			pr_info("  mem write [%08lX] : %02lX\n", addr, val);
			break;
		case 2:
			writew(val, reg);
			pr_info("  mem write [%08lX] : %04lX\n", addr, val);
			break;
		case 4:
			writel(val, reg);
			pr_info("  mem write [%08lX] : %08lX\n", addr, val);
			break;
		}
		iounmap(reg);
	}

	else {
		pr_info("  ioremap fail [%08lX]\n", addr);
		return -1;
	}

	return 0;
}


int mem_dump(unsigned long addr, int size)
{
	char *reg;
	long val;
	int i;

	reg = (char *)ioremap_nocache(addr, size);

	if (reg != NULL) {
		for (i = 0; i < size/4; i += 4 ) {
			val = readl(reg + i);
			pr_info("  mem read [%08lX] : %08lX\n", addr + i, val);
		}
		iounmap(reg);
	} else {
		pr_info("  ioremap fail [%08lX]\n", addr);
		return -1;
	}

	return 0;
}

ssize_t memd_proc_write(struct file *file, const char *buffer, size_t count, loff_t *pos)
{
	char buff[256];
	unsigned long len = count;
	int ret;
	int cmd;
	unsigned long addr;
	unsigned long val;

	if (copy_from_user(buff, buffer, len))
		return -EFAULT;

	buff[len - 1] = '\0';

#ifdef debug
	pr_info("Recv Command : [%s] (len = %ld)\n", buff, len);
#endif

	if (cut_buff(buff, &cmd, &addr, &val))
		return -EINVAL;

	switch (cmd) {
	case D_MEM_READ:
		ret = mem_read(addr, 1);
		break;
	case D_MEM_READ_W:
		ret = mem_read(addr, 2);
		break;
	case D_MEM_READ_D:
		ret = mem_read(addr, 4);
		break;
	case D_MEM_WRITE:
		ret = mem_write(addr, val, 1);
		break;
	case D_MEM_WRITE_W:
		ret = mem_write(addr, val, 2);
		break;
	case D_MEM_WRITE_D:
		ret = mem_write(addr, val, 4);
		break;
	case D_MEM_DUMP:
		ret = mem_dump(addr, 0x80);
		break;
	default:
		pr_info("  Fatal error\n");
		ret = -1;
	}

	if (ret)
		return -EINVAL;

	return len;
}


static const struct file_operations entry_proc_fops = {
	.owner = THIS_MODULE,
	.write  = memd_proc_write,
};

static int memd_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create_data(PROCNAME, 0666, NULL, &entry_proc_fops, NULL);
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

