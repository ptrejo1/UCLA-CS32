class calc:

	def addition(x,y):
		added = x+y
		print(added)

	def subtract(x,y):
		sub = x-y
		print(sub)

calc.addition(1,3)

# make your own py modules is just creating a script and importing
# it into the script your working, ensure they are in same directory
# if you want entire python to recognize module place it in python
# packages in lib

x = [1,2,3]

# will give last element in list
# - sign means starting from back
print(x[-1])

import csv

with open('example.csv') as csvfile:
	readCSV = csv.reader(csvfile, delimiter=',')

	dates = []
	colors = []

	for row in readCSV:
		color = row[3]
		date = row[0]

		dates.append(date)
		colors.append(color)

	print(dates)
	print(colors)

	try:
		whatColor = input('What color for what date?')

		if whatColor in colors:
			coldex = colors.index(whatColor)
			theDate = dates[coldex]
		else:
			print('color not found')

	# can also specify which error to catch
	except Exception as e:
		print(e)

