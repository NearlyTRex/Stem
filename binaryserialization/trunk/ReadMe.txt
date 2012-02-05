BinarySerialization provides SerializationContext and DeserializationContext subclasses that translate to and from compact binary data. Both big endian and little endian representations are supported.

BinarySerialization is most efficient when data is contained in structures. No extra data is written to represent the boundaries of a structure or its keys. Each array has four bytes of overhead for its item count. Dictionaries are the least efficient; each one has eight bytes of overhead, and for each item written, a number of bytes equal to the length of the key string plus five.

Dictionary binary format:

struct dictionary {
	// dictionary start
	uint32_t count;
	uint32_t endOffset; // from dictionary start
	struct {
		char * key;
		uint32_t valueOffset; // from dictionary start
	} entries[count];
	...values;
};
