#Test sample for OZWriteService

from thrift.transport import TTransport
from thrift.transport import TSocket
from thrift.protocol import TBinaryProtocol
import OZWriteService
import OZReadService
import OZTravService
	
class OZoneWriteClient:
	def __init__(self, host, port):
		socket = TSocket.TSocket(host, port)
		#self.transport = TTransport.TFramedTransport(socket)
		self.transport = TTransport.TBufferedTransport(socket)
		protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.client = OZWriteService.Client(protocol)
		self.transport.open()
		
	def __del__(self):
		self.transport.close()
		del self.client
		del self.transport
		
	def put(self, value):
		self.client.put(value)

	def puts(self, values):
		self.client.puts(values)

class OZoneReadClient:
	def __init__(self, host, port):
		socket = TSocket.TSocket(host, port)
		#self.transport = TTransport.TFramedTransport(socket)
		self.transport = TTransport.TBufferedTransport(socket)
		protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.client = OZReadService.Client(protocol)
		self.transport.open()
		
	def __del__(self):
		self.transport.close()
		del self.client
		del self.transport
		
	def get(self, value):
		return self.client.get(value)

	def gets(self, values):
		return self.client.gets(values)

class OZoneTravClient:
	def __init__(self, host, port):
		socket = TSocket.TSocket(host, port)
		#self.transport = TTransport.TFramedTransport(socket)
		self.transport = TTransport.TBufferedTransport(socket)
		protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.client = OZTravService.Client(protocol)
		self.transport.open()
		
	def __del__(self):
		self.transport.close()
		del self.client
		del self.transport
		
	def next_kv(self, cur=0):
                pair = self.client.next_kv(cur)
		return (pair.key, pair.value, pair.cur)

