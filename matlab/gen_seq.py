#!/usr/bin/env python

from random import shuffle

def check_seq(seq):
    for i in range(1,len(seq)):
        if seq[i-1] == seq[i]:
            return False
    return True

def gen_seq(nb_flashes):
    seq = []
    r = range(1,17)
    shuffle(r)
    for i in r:
        seq.append(i)
    for i in range(nb_flashes - 1):
        v = range(1,17)
        shuffle(v)
        while v[0] == seq[-1]:
            shuffle(v)
        for j in v:
            seq.append(j)
    return seq

def gen_line(nb_flashes):
    line = '{'
    seq = gen_seq(nb_flashes)
    while not check_seq(seq):
        seq = gen_seq(nb_flashes)
    for i in seq:
        line += ' '+str(i)+','
    line = line[:len(line)-1] + '}'
    return line

def gen_header(name, nb_lines, nb_flashes):
    line = 'unsigned int ' + name + '[' + str(nb_lines) + '][' + str(16*nb_flashes) + '] = {'
    for i in range(nb_lines):
        line += gen_line(nb_flashes) + ',\n'
    line = line[:len(line)-2] + ';'
    return line

print gen_header('TRAIN_STIMULATION', 256, 15)
print gen_header('FREE_STIMULATION', 256, 5)
