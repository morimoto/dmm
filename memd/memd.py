#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
# -----------------------------------------------------------------------------
# memd.py: Wrapper script for memd
#
# Copyright (c) 2019 Yuya Hamamachi <yuya.hamamachi.sx@renesas.com>
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# import
# -----------------------------------------------------------------------------
import logging
import math
import optparse
import os
import sys
import textwrap


# -----------------------------------------------------------------------------
# Class
# -----------------------------------------------------------------------------
class Memd(object):
    # Class Variables
    ACCESS_SIZE_DICT = {"b": 1, "w": 2, "l": 4, "": 4}
    DUMP_CMD_DICT = {1: "r", 2: "rw", 4: "rd"}
    EDIT_CMD_DICT = {1: "w", 2: "ww", 4: "wd"}
    SCRIPT_NAME = os.path.basename(__file__)
    DEV_FILE = "/proc/reg"

    COLOR_BLUE = "\x1b[34m"
    COLOR_RESET = "\x1b[0m"

    def __init__(self):
        self.help_msg = textwrap.dedent("""\
            Wrapper script for memd: Memory editor/dumper
                md   : Memory dumper
                me   : Memory editor
                help : Show this message
            If you want to know detail of each command
                {script_name} help md
                {script_name} help me
        """.format(script_name=self.SCRIPT_NAME))

    def get_access_size(self, cmd):
        access_size_option = cmd.split(".")[1] if ("." in cmd) else ""
        access_size = self.ACCESS_SIZE_DICT[access_size_option]
        return access_size

    def run(self, args):
        self.print_help_and_exit()

    def mem_dump(self, access_size, address):
        value = ""
        with open(self.DEV_FILE, "r+") as f:
            cmd = "{command} 0x{addr:x}".format(
                    command=self.DUMP_CMD_DICT[self.access_size],
                    addr=int(address, 16),)
            logging.debug(cmd)
            f.write(cmd)
            value = f.read()
        return value

    def mem_edit(self, access_size, address, value, mask=None):
        with open(self.DEV_FILE, "r+") as f:
            if mask is not None:
                read_data = int(self.mem_dump(access_size, address), 16)
                mask = int(mask, 16)
                value = hex((read_data & ~mask) | (int(value, 16) & mask))

            cmd = "{command} 0x{addr:x} {data}".format(
                    command=self.EDIT_CMD_DICT[self.access_size],
                    addr=int(address, 16),
                    data=value)
            logging.debug(cmd)
            f.write(cmd)

    def print_help_and_exit(self):
        print(self.help_msg)
        quit()


class MemdDump(Memd):
    def __init__(self):
        self.help_msg = textwrap.dedent("""\
            {script_name} md[.b, .w, .l] addr [len] [-cs]
                -c   : dump in 1 column
                -s   : silence output
                addr : address
                len  : length
            command list
                md.l : Read 32 bit
                md.w : Read 16 bit
                md.b : Read  8 bit
                md   : Same as 'md.l'
            example")
                {script_name} md.b 0x12345678
                {script_name} md.w 0x12345678 0x20
                {script_name} md   0x12345678 32
            """.format(script_name=self.SCRIPT_NAME))

    def parse_cmd(self, args):
        logging.debug("parse_memd_dump")
        parser = optparse.OptionParser()
        parser.add_option('-c', action="store_true", default=False)
        parser.add_option('-s', action="store_true", default=False)
        self.opts, args = parser.parse_args(args)

        if len(args) < 2 or 3 < len(args):
            self.print_help_and_exit()

        # Get Access_size
        self.access_size = self.get_access_size(args[1])

        # Get top_addr and dump_size
        self.top_address = args[1]
        self.dump_size = args[2] if (len(args) == 3) else "0x80"

        if "0x" in self.dump_size:
            self.dump_size = int(self.dump_size, 16)
        else:
            self.dump_size = int(self.dump_size)

    def run(self, args):
        self.parse_cmd(args)
        logging.debug(self.top_address, " : ", self.dump_size)

        values = []
        loop_count = math.ceil(self.dump_size / self.access_size)
        for i in range(loop_count):
            addr = hex(int(self.top_address, 16) + i*self.access_size)
            values.append(self.mem_dump(self.access_size, addr))

        logging.debug(values)
        self.print_result(values)

    def print_result(self, values):
        values_aligned = ""
        output = ""
        addr_offset = int(self.top_address[-1], 16) // self.access_size
        padding = "".join([" " for _ in range(self.access_size*2)])

        # align values
        for _ in range(addr_offset):
            values_aligned += "{} ".format(padding)
        for i, value in enumerate(values):
            values_aligned += "{} ".format(value)
            if ((i+addr_offset+1) % (16 // self.access_size)) == 0:
                values_aligned += "\n"
        values_aligned += "" if (values_aligned[-1] == "\n") else "\n"

        if self.opts.c is True and self.opts.s is True:
            output = "\n".join([i for i in values_aligned.split()]) + "\n"

        elif self.opts.c is True:
            for i, value in enumerate(values_aligned.split()):
                output += "{color}0x{addr:x}{reset}   {val}\n".format(
                           addr=int(self.top_address, 16)+i*self.access_size,
                           val=value,
                           color=self.COLOR_BLUE,
                           reset=self.COLOR_RESET,
                           )

        elif self.opts.s is True:
            output = values_aligned

        else:
            header = "             "
            for i in range(0, 16, self.access_size):
                header += "{color}{num:x}{reset}{padding}".format(
                           num=i,
                           color=self.COLOR_BLUE,
                           reset=self.COLOR_RESET,
                           padding=padding,)
            output = header + "\n"
            for i, line in enumerate(values_aligned.splitlines()):
                output += "{color}0x{addr:x}{reset}   {val}\n".format(
                           addr=(int(self.top_address, 16)+i*16) & 0xfffffff0,
                           val=line,
                           color=self.COLOR_BLUE,
                           reset=self.COLOR_RESET,)

        print(output, end="")


class MemdEdit(Memd):
    def __init__(self, args=[]):
        self.help_msg = textwrap.dedent("""\
            {script_name} me[.b, .w, .l] addr data
                addr : address
                data : data
            command list
                me.l : Write 32 bit
                me.w : Write 16 bit
                me.b : Write  8 bit
                me   : Same as 'me.l'
            example
                {script_name} me.b 0x12345678 0x01
                {script_name} me.w 0x12345678 0x0123
                {script_name} me   0x12345678 0x01234567

            advanced usage
                You can use mask and set by -m option
                    {script_name} me -m 0x12345678 0x0000FFFF 0x00004567

                You can continuously write data to sequential address.
                    {script_name} me.w 0x12345678 0x1111 0x2222 0x3333
                        # [0x12345678] 0x1111
                        # [0x1234567a] 0x2222
                        # [0x1234567c] 0x3333
            """.format(script_name=self.SCRIPT_NAME))

    def parse_cmd(self, args):
        logging.debug("parse_memd_edit")
        self.mask = None

        parser = optparse.OptionParser()
        parser.add_option('-m', action="store_true", default=False)
        self.opts, args = parser.parse_args(args)

        if len(args) < 3:
            self.print_help_and_exit()

        # Get Access_size
        self.access_size = self.get_access_size(args[1])

        # Get top_addr, value(s), and mask
        self.top_address = args[1]
        if self.opts.m is True:
            self.mask = args[2]
            args.pop(2)
        self.values = args[2:]

    def run(self, args):
        self.parse_cmd(args)
        for i, value in enumerate(self.values):
            addr = hex(int(self.top_address, 16) + i*self.access_size)
            self.mem_edit(self.access_size, addr, value, self.mask)


# -----------------------------------------------------------------------------
# main
# -----------------------------------------------------------------------------
def main(args):
    # Enable debug mode
    if "--debug" in args:
        args.remove("--debug")
        formatter = '%(levelname)s : %(asctime)s : %(message)s'
        logging.basicConfig(level=logging.DEBUG, format=formatter)

    args.pop(0)  # remove script_name from args.

    try:
        if args[0] == "help":
            mode = args[1].split(".")[0]
            args.clear()
        else:
            mode = args[0].split(".")[0]
    except IndexError:
        mode = ""

    if mode == "md":
        memd = MemdDump()
    elif mode == "me":
        memd = MemdEdit()
    else:
        memd = Memd()

    memd.run(args)


if __name__ == '__main__':
    main(sys.argv)
