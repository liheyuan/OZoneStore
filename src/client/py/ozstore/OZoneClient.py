#Test sample for OZWriteService

from thrift.transport import TTransport
from thrift.transport import TSocket
from thrift.protocol import TBinaryProtocol
import OZWriteService
	
class OZoneWriteClient:
	def __init__(self, host, port):
		socket = TSocket.TSocket(host, port)
		self.transport = TTransport.TFramedTransport(socket)
		protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.client = OZWriteService.Client(protocol)
		self.transport.open()
		
	def __del__(self):
		self.transport.close()
		
	def put(self, key, value):
		self.client.put(key, value)

		
