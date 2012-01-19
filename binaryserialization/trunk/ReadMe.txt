Dictionary binary format:

struct dictionary {
	uint32_t count;
	uint32_t endOffset; // from dictionary start (4 bytes ago)
	struct {
		char * key;
		uint32_t valueOffset; // from dictionary start
	} entries[count];
	...values;
};
