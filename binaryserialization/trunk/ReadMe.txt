BinarySerialization provides SerializationContext and DeserializationContext subclasses that translate to and from compact binary data. Both big endian and little endian representations are supported. All binary serialized data starts with a signature of either "Stem" (if big endian) or "metS" (if little endian).

Primitive types are written directly with no extra data. This includes boolean (written as 1 byte), int8, uint8, int16, uint16, int32, uint32, int64, uint64, float, double, and fixed16_16. Enumerations are written as int32. Bitfields are written as integers of the appropriate size.

Strings are written directly with a single terminating byte. Strings cannot contain null characters; if 0 bytes are needed, use a blob instead.

Nullable strings start with a control byte. If the control byte is 0x01, the string is written directly afterward with a null terminator. If the control byte is 0x00, the string is considered NULL, and no other bytes are written. For strings that don't need a possible NULL value, writing non-nullable strings is more efficient by one byte. Control byte values other than 0x00 and 0x01 are invalid and will return BINARY_SERIALIZATION_ERROR_INVALID_NULLABLE_STRING_CONTROL_BYTE when read back.

Blobs are written starting with a 32-bit unsigned length value, followed by the raw binary data contained within. There is no terminating byte for a blob. If the length value is 0xFFFFFFFF, the blob is considered NULL. Blob data sizes that don't fit into a 32-bit integer are not currently supported.

Structures have no overhead. Keys, length, and boundaries are inferred by API usage rather than stored in binary data. Because of this, structures are the most efficient way for data to be serialized, and should be used for anything with a fixed length and known order (or those things can be inferred the data as it's being read).

Arrays start with a 32-bit unsigned item count. The end of an array is not marked, and is inferred from API usage. Because of this, it's not possible to partially read an array, then skip to the end and continue reading the rest of the file; an array's elements must be exhausted before calling endArray(), or BINARY_SERIALIZATION_ERROR_ARRAY_NOT_FULLY_READ will be returned.

Dictionaries start with a 32-bit unsigned item count, followed by a 32-bit total byte count measured from the beginning of the item count to the end of the last item. After the 8 header bytes is a list of pairs of null-terminated key strings and 32-bit byte offsets for the location of the corresponding value relative to the beginning of the dictionary. In struct form, they look like this:

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

TODO: Dictionary value offsets should be relative to the beginning of the value list, not the beginning of the dictionary, if possible
TODO: Remove useless little endian mode?
TODO: Make blobs nullable (must update jsonserialization too!)
TODO: For the above two changes to make sense, the binary format itself needs a version number. Repurpose 'Stem' signature and add version field.