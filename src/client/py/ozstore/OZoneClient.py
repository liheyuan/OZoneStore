#Test sample for OZWriteService

def test_write(host, port):
	#TSocket, TFramedTransport, TBinaryProtocol
	socket = TSocket.TSocket(host, port)
	transport = TTransport.TFramedTransport(socket)
	protocol = TBinaryProtocol.TBinaryProtocol(transport)
	client = OZWriteService.Client(protocol)
	transport.open()

if __name__ == "__main__":
	
