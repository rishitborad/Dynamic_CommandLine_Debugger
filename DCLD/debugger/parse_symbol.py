#!/usr/bin/python

# Command line input support
import sys
import re

# GUI to open a file required
from Tkinter import *
from tkFileDialog import askopenfilename
from itertools import imap

Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing
filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
#print(filename)

if filename.endswith('.lst'):

    # Open a file
    fo = open(filename, "r+")
    myarray = fo.read().split('\n')
    # print "Name of the file: ", fo.name

    del myarray[0]
    fu = open("map.txt","w+")

    for line in myarray:
        if (".data" in line and "::" not in line) or (".bss" in line and "::" not in line) or (".text" in line and "(" in line and "::" not in line):

            fu.write(line)
            fu.write('\n')
            # break

    fu.write('\0')

    fo.close()
    fu.close()


    # cleaning the file to remove the unwanted columns
    infile = r"map.txt"
    outfile = r"LUT.txt"

    delete_list = [" F "," l "," g "," w "," O "]
    fin=open(infile,"r+")
    fout = open(outfile,"w+")
    for line in fin:
        for word in delete_list:
            line = line.replace(word, "")
        fout.write(line)
        # break

    # Close opened file
    fin.close()
    fout.close()

    # delete the address offset column as we do not need it
    f = open("LUT.txt", "r")
    g = open("map.txt", "w")

    for line in f:
        if line.strip():
            g.write(" ".join(line.split()[0:2])+" "+" ")
        if line.strip():
           g.write(" ".join(line.split()[3:]) + "\n")

    f.close()
    g.close()

    # Adding the flags (a,v,i) for variables, void functions and functions with arguments respectively
    f = open("map.txt", "r+")
    g = open("LUT.txt", "w+")
    
    for line in f:
        g.write("0x")
        g.write(line)

    f.close()
    g.close()
    
    f = open("LUT.txt", "r+")
    g = open(sys.argv[1], "w+")
    
    for line in f:
        if ".data" in line or ".bss" in line:
            columns = line.split(" ")
            columns.insert(0, "a")
            g.write(" ".join(columns))

        if ".text" in line:
             if "()" in line:
                columns = line.split(" ")
                columns.insert(0, "v")
                g.write(" ".join(columns))
                continue

             if "(" in line:
                columns = line.split(" ")
                columns.insert(0, "i")
                g.write(" ".join(columns))

    f.close()
    g.close()

elif filename.endswith('.map'):

    fu= open("map.txt","w+")

    with open(filename, "r") as f:
        rows = imap(str.split, f)
        for row in rows:

                try:
                    if row[0].startswith(".data.g"):
                        a, b = row[0].rsplit(".", 1)
                        a, b = row[0].rsplit(".", 1)
                        fu.write(".data")
                        fu.write('\t''\t''     ')
                        fu.write((next(rows)[0], a, b).__getitem__(0))
                        fu.write('\t''\t''     ')
                        #fu.write((next(rows)[0], a, b).__getitem__(1))
                        #fu.write('\t')
                        fu.write((next(rows)[0], a, b).__getitem__(2))
                        fu.write('\n')
                except IndexError:
                    continue

            #if("*(.bss*)" in row):
                try:
                    if row[0].startswith(".bss.g"):
                        a, b = row[0].rsplit(".", 1)
                        a, b = row[0].rsplit(".", 1)
                        fu.write(".bss")
                        fu.write('\t''\t''     ')
                        fu.write((next(rows)[0], a, b).__getitem__(0))
                        fu.write('\t''\t''     ')
                        #fu.write((next(rows)[0], a, b).__getitem__(1))
                        #fu.write('\t')
                        fu.write((next(rows)[0], a, b).__getitem__(2))
                        fu.write('\n')
                except IndexError:
                    continue

    with open(filename, "r") as f:
        for line in f:
            if(".data." in line):
                data = next(f)
                if((".o" not in data) and (".data" not in data)and ("::" not in data) and ("*fill*" not in data)):
                    fu.write(".data")
                    fu.write(data)

    with open(filename, "r+") as f:
        for line in f:
            if(".text" in line):
                text = next(f)
                text = next(f)
                if((".o" not in text) and (".text" not in text)and ("::" not in text) and (".rodata" not in text)
                   and (".isr" not in text)and ("size before relaxing" not in text)and (".bss" not in text) and ("*fill*" not in text)):
                    fu.write(".text")
                    fu.write(text)

    with open(filename, "r+") as f:
        for line in f:
            if(".text" in line):
                text = next(f)
                if((".o" not in text) and (".text" not in text)and ("::" not in text) and (".rodata" not in text)
                   and (".isr" not in text)and ("size before relaxing" not in text)and (".bss" not in text) and ("*fill*" not in text)):
                    fu.write(".text")
                    fu.write(text)

    fu.close()
    f.close()

    f=open("map.txt", "r+")
    g=open("LUT.txt","w+")

    for line in f:
        if line.strip():
            g.write("  ".join(line.split()[0:])+" "+"\n")

    f.close()
    g.close()

    g = open("LUT.txt", "r+")
    f = open(sys.argv[1], "w+")
    f.write("  ")
    for line in g:
        if ".data" in line or ".bss" in line:
            columns = line.split("  ")
            columns.insert(0, "a")
            f.write("  ".join(columns)+"  ")

        if ".text" in line:
             if "()" in line:
                columns = line.split("  ")
                columns.insert(0, "v")
                f.write("  ".join(columns)+"  ")
                continue

             if "(" in line:
                columns = line.split("  ")
                columns.insert(0, "i")
                f.write("  ".join(columns)+"  ")

    f.close()
    g.close()