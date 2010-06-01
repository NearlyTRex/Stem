#ifndef __SERIALIZATION_SHARED_H__
#define __SERIALIZATION_SHARED_H__

#define enumKV(value) #value, value

// No error
#define SERIALIZATION_ERROR_OK 0

// Incorrect API usage, such as mismatched begin/end structure/dictionary/array calls
#define SERIALIZATION_ERROR_INVALID_OPERATION 1

// Other errors defined by concrete serialization modules

#endif
