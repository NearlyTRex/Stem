dynamictypes provides a system for working with arbitrary heterogeneous hierarchical data in memory:
- DataValue is a typed container that can store both primitive and composite types.
- DataArray stores a dynamically resizable list of DataValues with an integer index.
- AssociativeArray stores a dynamically resizable list of DataValues, along with a non-unique key associated with each one.
- HashTable allows for quick lookup of DataValues with unique keys.
- DataSerializationContext and DataDeserializationContext can be used with anything that implements the serialization API to serialize to/from the DataValue format in memory.
- DataSerializer implements the serialization API to allow DataValue to be interchanged with other serialization types.
