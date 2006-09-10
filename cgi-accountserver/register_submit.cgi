#! /usr/bin/python

import cgi
import sys
import os
import string
import cgitb; cgitb.enable()
import db
import settings

print "Content-Type: text/html"
print ""

header = """
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>Account Creation Results</title>
</head>
<body>
"""

footer = """
</body>
</html>
"""

form = cgi.FieldStorage()
if form.has_key("mod"):
	mod = form["mod"].value
else:
	mod = os.environ.get('QUERY_STRING','')

if mod:
	query = '?'+mod
else:
	query=''

errorvar="<br><a href=\"register.cgi"+query+"\">Return To Account Creation Form</a>"

success = False

try:
	dbconn = db.connect(settings.dbconfig, mod)
	
	username = dbconn.check_string( form.getvalue("username",'') )
	password = dbconn.check_string( form.getvalue("password",'') )
	faction = dbconn.check_string( form.getvalue("faction",'') )
	type = dbconn.check_string( form.getvalue("type",'') )
	
	if not dbconn.check_password(username, password):
		raise db.DBError,"Error password for username "+username+" does not match our records"
	
	dbconn.modify_account(username, type, faction)
	success = True
except db.DBError:
	success = False
	failed_error = sys.exc_value

print header
print '<br>'
if success:
	print 'Download functional'
	print '<a href="http://vegastrike.sourceforge.net/cgi-bin/vegastrike.config?username='+username+'&password='+password+'&mod='+mod+'">vegastrike.config</a>'
	print 'to put in your vegastrike folder that has your appropriate login and password<br>'
else:
	print '<b>Registration Error</b>: ' + str(failed_error)
	print errorvar

print footer
