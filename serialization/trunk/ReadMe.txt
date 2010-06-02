= Overview =

Serialization defines a generic interface for reading, writing, and validating structured data. It is intended to be used by model objects that wish to be serializable, facilitated by concrete subclasses of SerializationContext and DeserializationContext.

= Structure =

Structured data understood by serialization is categorized into three container types: array, structure, and dictionary. How these are represented in serialized form is up to individual serializers, but each one has a particular set of semantics:

	* Arrays are linear lists of zero or more values with no associated keys. Any type of value can be stored in any array. Values are read and written in order. Keys are not semantically meaningful for reading and writing array values (and need not be unique), but they may be stored in some serialized formats.

	* Structures are strictly-ordered lists of key-value pairs. Keys must be unique, and are not necessarily stored in serialized form. When deserializing from a format that stores structure keys, keys may be out of order, but the directives to read and write values must always come in the same order.

	* Dictionaries are optionally-ordered lists of key-value pairs. Keys are necessarily stored in serialized form, and do not necessarily need to be unique (though dictionaries with non-unique keys must be read a particular way; see below). There are two different calling conventions you can use to read a dictionary depending on whether you want to treat it as ordered or unordered. Behavior is undefined if the two ways of reading dictionaries are intermixed for the same dictionary.

		* To read a dictionary in an ordered manner, call readNextDictionaryKey() before each value read function, and pass its return value as the key parameter to the value read call. Ordered reading allows for non-unique keys.

		* To read an unordered dictionary, you must know the keys you wish to read ahead of time. Do not call readNextDictionaryKey(); simply call value read functions with the keys you wish to read, in any order. To check whether a key is present in an unordered dictionary, you can call hasDictionaryKey(); note that it is an error to attempt to read a value with a key that is not present in the dictionary. Unordered reading does not allow for non-unique keys; behavior is undefined when reading a duplicated key.

= Examples =

Example usage of SerializationContext:

	context->beginStructure(context, "struct");
	context->writeInt8(context, "int8", -40);
	context->beginDictionary(context, "dict");
	context->writeUInt8(context, "uint8", 192);
	context->writeInt16(context, "int16", -30000);
	context->endDictionary(context);
	context->writeUInt16(context, "uint16", 65535);
	context->writeInt32(context, "int32", -100000);
	context->beginArray(context, "array");
	context->writeUInt32(context, "array_item", 100000);
	context->writeInt64(context, "array_item", -5000000000);
	context->writeUInt64(context, "array_item", 5000000000);
	context->endArray(context);
	context->writeFloat(context, "float", 0.25);
	context->writeDouble(context, "double", 1.5);
	context->writeString(context, "string", "Hello, world!");
	context->writeEnumeration(context, "first_enum", ENUM_VALUE_0, enumKV(ENUM_VALUE_0), enumKV(ENUM_VALUE_1), NULL);
	context->writeEnumeration(context, "second_enum", ENUM_VALUE_1, enumKV(ENUM_VALUE_0), enumKV(ENUM_VALUE_1), NULL);
	context->writeBitfield8(context, "bitfield8_1", 0x1E, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield8(context, "bitfield8_2", 0x05, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield16(context, "bitfield16_1", 0x001E, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield16(context, "bitfield16_2", 0x0005, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield32(context, "bitfield32_1", 0x0000001Eu, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield32(context, "bitfield32_2", 0x00000005u, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield64(context, "bitfield64_1", 0x000000000000001Eull, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->writeBitfield64(context, "bitfield64_2", 0x0000000000000005ull, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	context->endStructure(context);

Example usage of DeserializationContext, to read back the above data:

	count = context->beginStructure(context, "struct");
	// count: 18
	int8 = context->readInt8(context, "int8");
	// int8: -40
	
	count = context->beginDictionary(context, "dict");
	// count: 2
	key = context->readNextDictionaryKey(context);
	// key: "uint8"
	uint8 = context->readUInt8(context, key);
	// uint8: 192
	key = context->readNextDictionaryKey(context);
	// key: "int16"
	int16 = context->readInt16(context, key);
	//int16: -30000
	context->endDictionary(context);
	
	uint16 = context->readUInt16(context, "uint16");
	// uint16: 65535
	int32 = context->readInt32(context, "int32");
	// int32: -100000
	
	count = context->beginArray(context, "array");
	// count: 3
	uint32 = context->readUInt32(context, "array_item");
	// uint32: 100000
	int64 = context->readInt64(context, "array_item");
	// int64: -5000000000
	uint64 = context->readUInt64(context, "array_item");
	// uint64: 5000000000
	context->endArray(context);
	
	floatValue = context->readFloat(context, "float");
	// floatValue: 0.25
	doubleValue = context->readDouble(context, "double");
	// doubleValue: 1.5
	string = context->readString(context, "string");
	// string: "Hello, world!"
	enumValue = context->readEnumeration(context, "first_enum", enumKV(ENUM_VALUE_0), enumKV(ENUM_VALUE_1), NULL);
	// enumValue: ENUM_VALUE_0
	enumValue = context->readEnumeration(context, "second_enum", enumKV(ENUM_VALUE_0), enumKV(ENUM_VALUE_1), NULL);
	// enumValue: ENUM_VALUE_1
	uint8 = context->readBitfield8(context, "bitfield8_1", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint8: 0x1E
	uint8 = context->readBitfield8(context, "bitfield8_2", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint8: 0x05
	uint16 = context->readBitfield16(context, "bitfield16_1", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint16: 0x001E
	uint16 = context->readBitfield16(context, "bitfield16_2", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint16: 0x0005
	uint32 = context->readBitfield32(context, "bitfield32_1", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint32: 0x0000001Eu
	uint32 = context->readBitfield32(context, "bitfield32_2", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint32: 0x00000005u
	uint64 = context->readBitfield64(context, "bitfield64_1", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint64: 0x000000000000001E
	uint64 = context->readBitfield64(context, "bitfield64_2", "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", NULL);
	// uint64: 0x0000000000000005
	context->endStructure(context);

The serialized output of the above code might look like this with a binary serializer:

	0000: D8 00 00 00 02 75 69 6E 74 38 00 C0 69 6E 74 31 	.....uint8..int1
	0010: 36 00 8A D0 FF FF FF FF FF FE 79 60 00 00 00 03 	6.........y`....
	0020: 00 01 86 A0 FF FF FF FE D5 FA 0E 00 00 00 00 01 	................
	0030: 2A 05 F2 00 00 00 80 3E 00 00 80 3E 00 00 00 00 	*......>...>....
	0040: 48 65 6C 6C 6F 2C 20 77 6F 72 6C 64 21 00 00 00 	Hello, world!...
	0050: 00 00 00 00 00 01 05 03 00 1E 00 05 00 00 00 1E 	................
	0060: 00 00 00 05 00 00 00 00 00 00 00 1E 00 00 00 00 	................
	0070: 00 00 00 05                                     	....

In JSON:

	{
		"int8": -40,
		"dict": {
			"uint8": 192,
			"int16": -30000
		},
		"uint16": 65535,
		"int32": -100000,
		"array": [
			100000,
			-5000000000,
			5000000000
		],
		"float": 0.25,
		"double": 1.5,
		"string": "Hello, world!",
		"first_enum": "ENUM_VALUE_0",
		"second_enum": "ENUM_VALUE_1",
		"bitfield8_1": [
			"bit1",
			"bit2",
			"bit3",
			"bit4"
		],
		"bitfield8_2": [
			"bit0",
			"bit2"
		]
		"bitfield16_1": [
			"bit1",
			"bit2",
			"bit3",
			"bit4"
		],
		"bitfield16_2": [
			"bit0",
			"bit2"
		]
		"bitfield32_1": [
			"bit1",
			"bit2",
			"bit3",
			"bit4"
		],
		"bitfield32_2": [
			"bit0",
			"bit2"
		]
		"bitfield64_1": [
			"bit1",
			"bit2",
			"bit3",
			"bit4"
		],
		"bitfield64_2": [
			"bit0",
			"bit2"
		]
	}

In XML:

	<struct>
		<int8 value="-40"/>
		<dict>
			<uint8 value="192"/>
			<int16 value="-30000"/>
		</dict>
		<uint16 value="65535"/>
		<int32 value="-100000"/>
		<array>
			<array_item value="100000"/>
			<array_item value="-5000000000"/>
			<array_item value="5000000000"/>
		</array>
		<float value="0.25"/>
		<double value="1.5"/>
		<string value="Hello, world!"/>
		<first_enum value="ENUM_VALUE_0"/>
		<second_enum value="ENUM_VALUE_1"/>
		<bitfield8_1>
			<bit1/>
			<bit2/>
			<bit3/>
			<bit4/>
		</bitfield8_1>
		<bitfield8_2>
			<bit0/>
			<bit2/>
		</bitfield8_2>
		<bitfield16_1>
			<bit1/>
			<bit2/>
			<bit3/>
			<bit4/>
		</bitfield16_1>
		<bitfield16_2>
			<bit0/>
			<bit2/>
		</bitfield16_2>
		<bitfield32_1>
			<bit1/>
			<bit2/>
			<bit3/>
			<bit4/>
		</bitfield32_1>
		<bitfield32_2>
			<bit0/>
			<bit2/>
		</bitfield32_2>
		<bitfield64_1>
			<bit1/>
			<bit2/>
			<bit3/>
			<bit4/>
		</bitfield64_1>
		<bitfield64_2>
			<bit0/>
			<bit2/>
		</bitfield64_2>
	</struct>