'''
Created on 2011-11-25

@author: liheyuan
'''

from ozstore import OZoneClient
import random
from multiprocessing import Process
import threading 

def test_write_single():
    try:
        client = OZoneClient.OZoneWriteClient("127.0.0.1", 9090)
        for i in xrange(1024):
            client.put(str(i) * 1024)
    except Exception as e:
        print e
        
def test_write():
    ps = []
    for i in xrange(10):
        p = Process(target=test_write_single)
        ps.append(p)
        p.start()
    for p in ps:
        p.join()

def test_writes_single():
    try:
        client = OZoneClient.OZoneWriteClient("127.0.0.1", 9090)
        values = []
        for i in xrange(16):
            values.append(str(i) * 1024)
        client.puts(values)
    except Exception as e:
        print e

class ReadThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        try:
            client = OZoneClient.OZoneReadClient("127.0.0.1", 9090)
    	    for i in xrange(100):
                key = random.randint(0, 2000)
                try:
                    client.get(str(key))
                except:
                    print "err, ", key
                    pass
        except Exception as e:
            print "exception"
            print e

def test_read():
    ts = []
    for i in xrange(16):
        t = ReadThread()
        ts.append(t)
        t.start()
    for t in ts:
        t.join()

if __name__ == "__main__":
    test_write()
    #test_writes_single()
    #test_read()
    
