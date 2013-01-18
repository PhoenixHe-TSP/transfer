import webapp2
from google.appengine.api import channel, memcache

class GetTokenHandler(webapp2.RequestHandler):
    def post(self):
        id = self.request.get('id')
        if not id:
            return
        info = memcache.get(id)
        if not info:
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
    def post(self):
        id = self.request.get('from')
        info = memcache.get(id)
        if not info:
        #not possible yet...
            return
        info[1] = 1
        memcache.set(id, info)

class DisconnectHandler(webapp2.RequestHandler):
    def post(self):
        id = self.request.get('from')
        info = memcache.get(id)
        if not info:
        #not possible yet...
            return
        info[1] = 0
        memcache.set(id, info)

class ReceiveHandler(webapp2.RequestHandler):
    def post(self):
        id = self.request.get('id')
        msg = self.request.get('msg')
        if not id or not msg:
            return
        """ 
        my transfer protocol:
        op_code id [data]
        id: 4 bytes
        op_code: 1 byte
          D    send data to
          P    ping
          C    close
          p    pong
          n    id not found
          e    protocol error
        """ 
        if len(msg) < 5:
            if msg == 'C':
                info = memcache.get(id)
                if not info:
                    return
                info[1] = 0
                memcache.set(id, info)
            else:
                self.response.out.write('e' + msg)
            return

        opcode = msg[0]
        id_to = msg[1:5]
        info = memcache.get(id_to)
        if not info or not info[1]:
            self.response.out.write('n' + msg[1:])
        elif opcode in ['D','P','p']:
            channel.send_message(id_to, opcode + id + msg[5:])
        else:
            self.response.out.write('e' + msg)

app = webapp2.WSGIApplication([('/msg', ReceiveHandler),
                               ('/get', GetTokenHandler),
                               ('/_ah/channel/connected/', ConnectHandler),
                               ('/_ah/channel/disconnected/', DisconnectHandler)])
