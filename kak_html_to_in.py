#!/usr/bin/env python

# Script used to convert HTML code from http://www.kakuroconquest.com/ into input
# format used for kakuro solver (take the <tbody> element containing the puzzle).

import os, sys
try: 
    from BeautifulSoup import BeautifulSoup
except ImportError:
    from bs4 import BeautifulSoup

soup = BeautifulSoup(sys.stdin)
tbody = soup.tbody or soup
NUM_ROWS = len(tbody.findChildren('tr', recursive=False))
NUM_COLS = len(tbody.findChild('tr', recursive=False).findChildren('td', recursive=False))
table = []
sums = []
for i in range(NUM_ROWS):
	row = []
	tr = tbody.findChildren('tr', recursive=False)[i]
	for j in range(NUM_COLS):
		td = tr.findChildren('td', recursive=False)[j]
		cell_type = td['class']
		if cell_type == 'cellShaded':
			# solid cell
			pass
		elif cell_type == 'cellTotal':
			# cell containing the vertical/horizontal sum
			#print td
			h = td.findChild('div', attrs={'class': 'topNumberHelp'})
			v = td.findChild('div', attrs={'class': 'bottomNumberHelp'})
			h = int(h.text) if h else 0
			v = int(v.text) if v else 0
			sums.append('%d %d %d %d' % (i + 1, j + 1, h, v))
		elif cell_type == 'cellNumber':
			# cell to put the number in
			pass
		else:
			raise ValueError('cell_type[%d, %d] = %s' % (i, j, cell_type))
		row.append('w' if cell_type == 'cellNumber' else 'b')
	table.append(''.join(row))


print NUM_ROWS, NUM_COLS
print '\n'.join(table)
print '\n'.join(sums)
