// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "../ozcore/ozone.h"
#include "OZWriteService.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <server/TNonblockingServer.h>
#include <string>
#include <iostream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
using namespace std;

using boost::shared_ptr;

using namespace ozstore;

class OZWriteServiceHandler : virtual public OZWriteServiceIf
{
	public:
		OZWriteServiceHandler(const string& path)
		{
			if(ozwrite_open(&ow, path.c_str()))
			{
				throw TException("Open db fail.");
			}
		}

		void put(const std::string& key, const std::string& value)
		{
			//Try put
			int ret = ozwrite_put(&ow, key.c_str(), value.c_str());
			if(!ret)
			{
				return ;
			}
			// Not success
			OZException exp;
			switch(ret)
			{
				case 1:
					exp.why = "invalid handle";
					break;
				case 2:
					exp.why = "write key error";
					break;
				case 3:
					exp.why = "write value error";
					break;
				case 4:
					exp.why = "ftello fail";
				case 5:
					exp.why = "value length can't be zero";
					break;
				case 6:
					exp.why = "key length can't be zero";
					break;
				default:
					exp.why = "unknown error";
					break;
			}
			throw exp;
		}

		void puts(const std::vector<std::string> & key, const std::vector<std::string> & value)
		{
			// Your implementation goes here
			printf("puts\n");
		}

	private:
		OZWrite ow;
};

int main(int argc, char **argv)
{
	int port = 9090;
	int numThreads = 8;
	string path("/tmp/test_db");

	//Create TProcessor
	shared_ptr<OZWriteServiceHandler> handler(new OZWriteServiceHandler(path));
	shared_ptr<TProcessor> processor(new OZWriteServiceProcessor(handler));

	//Create TProtocolFactory
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	//Create ThreadManager
	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(numThreads);
	shared_ptr<ThreadFactory> threadFactory(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();

	//Create TNonblockingServer
	TNonblockingServer server(processor, protocolFactory, port, threadManager);
	cout << "OZWriteServer Starting..." << endl;
	server.serve();
	return 0;
}

