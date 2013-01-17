# -*- coding: utf-8 -*-

import webapp2
from google.appengine.api import channel, memcache

class GetTokenHandler(webapp2.RequestHandler):
	def get(self):
    id = self.request.get('id')
    if not id:
      return
    info = memcache.get(id)
    if not ret:
#register the id
      token = channel.create_channel(id)
      memcache.set(id, [token, 0])
    elif info[1]:
#the channel is already in use
      return
    else:
      token = info[0]
    self.response.out.write(token)

class ConnectHandler(webapp2.RequestHandler):
  def get(self):
    id = self.request.get('from')
    info = memcache.get(id)
    if not info:
#not possible yet...
      return
    info[1] = 1
    memcache.set(id, info)

class DisconnectHandler(webapp2.RequestHandler):
  def get(self):
    id = self.request.get('from')
    info = memcache.get(id)
    if not info:
#not possible yet...
      return
    info[1] = 0
    memcache.set(id, info)

class ReceiveHandler(webapp2.RequestHandler):
  def send(self, opcode, id, msg):
	def post(self):
		id = self.request.get('id')
    msg = self.request.get('msg')
		if not token or not msg:
			return
""" 
my transfer protocol:
op_code id [data]
id: 4 bytes, server's id is 0
op_code: 1 byte
  I    init id
  D    send data to
  P    ping
  p    pong
  s    successful requested one id
  c    id conflicts
  n    id not found
  t    protocol error
""" 
    if len(msg) < 5:
      return
    opcode = msg[0]
    id_to = msg[1:5]
    info = memcache.get(id_to)
    if not info[1]:
      d
      
    if opcode == 'D':       #data

    
app = webapp2.WSGIApplication([('/msg', ReceiveHandler),
									 ('/get', GetTokenHandler)],
                   ('/_ah/channel/connected', ConnectHandler),
                   ('/_ah/channel/disconnected', DisconnectHandler)])

if __name__ == '__main__':
	run_wsgi_app(app)
