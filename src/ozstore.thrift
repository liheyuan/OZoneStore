namespace cpp ozstore
namespace py ozstore


exception OZException {
	1:string why
}

service OZReadService {
	
	string get(1:string key) throws (1:OZException ouch),

	list<string> gets(1:list<string> keys) throws (1:OZException ouch)

}

service OZWriteService{
	
	/* void put(1:string key, 2:string value) throws (1:OZException ouch), */
	#key was auto-generated (increcemental)
	void put(1:string value) throws (1:OZException ouch),

	/* #void puts(1:list<string> key, 2:list<string> values) throws (1:OZException ouch) */
	#key was auto-generated (increcemental)
	void puts(1:list<string> values) throws (1:OZException ouch)
}

struct TravPair {

    # Key
    1:string key,

    # Value
	2:string value,

	# Cur
	3:i32 cur = 0
}

service OZTravService {

    # Traverse, Pair is input/output
	TravPair next_kv(1:i32 cur)
}
