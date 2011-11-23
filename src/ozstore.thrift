namespace cpp ozstore
namespace py ozstore


exception OZException{
	1:string why
}

service OZReadService{
	
	string get(1:string key) throws (1:OZException ouch),

	list<string> gets(1:list<string> keys) throws (1:OZException ouch)

}

service OZWriteService{
	
	void put(1:string key, 2:string value) throws (1:OZException ouch),

	void puts(1:list<string> key, 2:list<string> value) throws (1:OZException ouch)
}
