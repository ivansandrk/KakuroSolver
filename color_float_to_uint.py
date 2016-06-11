#!/usr/bin/env python

import os, sys, re

# pattern = r'(glColor3f)?\(([01]\.\d)f, ([01]\.\d)f, ([01]\.\d)f\)'
pattern = r'{([01]\.\d)f, ([01]\.\d)f, ([01]\.\d)f}'

data = '''{1.0f, 0.0f, 0.0f}
{0.0f, 1.0f, 0.0f}
{0.0f, 0.0f, 1.0f}
{1.0f, 1.0f, 0.0f}
{1.0f, 0.0f, 1.0f}
{0.0f, 1.0f, 1.0f}
{0.5f, 0.5f, 0.5f}
{0.5f, 0.5f, 1.0f}
{1.0f, 0.6f, 0.3f}
{1.0f, 1.0f, 1.0f}
{0.0f, 0.0f, 0.0f}'''

def str_to_floats(string):
	match = re.search(pattern, string)
	if not match:
		raise ValueError('Couldn\'t match: ' + string)
	return float(match.group(1)), float(match.group(2)), float(match.group(3))

def float_to_uint8(f):
	return int(0xFF * f)

def str_to_uint32(string):
	r, g, b = [float_to_uint8(x) for x in str_to_floats(string)]
	col = r << 16 | g << 8 | b
	return col

for line in data.split('\n'):
	try:
		print '0x%06X' % str_to_uint32(line)
	except:
		print 'OK'

