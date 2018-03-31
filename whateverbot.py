# bot 

from bs4 import BeautifulSoup
from urlparse import urlparse 

import praw
import time
import re 
import requests
import bs4


# Update variable with proper path
path = '/Users/phoenixtrejo/'

header = '**Whatever I end up deciding**\n'
footer = '\n*bot by u/boootbootbot* '


def authenticate():
	print 'Authenticating...\n'
	reddit = praw.Reddit('whateverbot', user_agent='bot does whatever')
	print 'Authenticated as {}\n'.format(reddit.user.me())
	return reddit

def fetchdata(url):
	r = requests.get(url)
	soup = BeautifulSoup(r.content, 'html.parser')

	tag = soup.find('p')

	data = ''

	while True:
		if isinstance(tag, bs4.element.Tag):
			if (tag.name == 'h2'):
				break
			if (tag.name == 'h3'):
				tag = tag.nextSibling
			else:
				data = data + '\n' + tag.text
				tag = tag.nextSibling	
		else:
			tag = tag.nextSibling

	return data

def run_explainbot(reddit):

	print 'Getting 250 comments... \n'
	