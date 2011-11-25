'''
Created on 2011-11-25

@author: liheyuan
'''

from ozstore import OZoneClient
import random
from multiprocessing import Process

def test_write_single():
    try:
        client = OZoneClient.OZoneWriteClient("127.0.0.1", 9090)
        for i in xrange(1024):
            client.put(str(random.randint(0, 10000000)), "a" * 1024)
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
    

if __name__ == "__main__":
    test_write()