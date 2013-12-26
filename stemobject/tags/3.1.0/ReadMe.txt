StemObject serves as the base for Stem's object system. The object system is very informal and done largely by these conventions:

	* Class and instance methods follow the naming convention ClassName_methodName().

	* All instance methods receive, as their first parameter, self_type * self. This is the instance on which the method is being called.

	* All instance methods have a corresponding function pointer field in the object struct. These function pointers are set to their corresponding function implementations in <Name>_init().

	* All class headers should contain the following line (adjusted for class name) before any #include statements for other Stem objects: typedef struct ClassName ClassName;

	* Object struct contents are #defined as ClassName_structContents in the class's header. Subclasses (which should #define their struct contents in the same way) include their superclass's structContents as the first thing in their own structContents.

	* Method overriding is done by implementing a matching Subclass_method in the subclass, and setting the function pointer in the struct to point to that method in <Name>_init().

	* Calls to superclass instance methods take the form: call_super(methodName, self, ...); (see notes about call_super below)

	* All subclass objects must call their superclass's <Name>_init() method as the first thing in the subclass's <Name>_init(). Use the call_super macro for this.

	* All subclass objects must call their superclass's <Name>_dispose() method as the last thing in the subclass's <Name>_dispose().

	* All classes must contain the following methods:

		* <Name>_create(): class method which allocates, initializes, and returns an object instance. This method should set the object struct's "allocated" field to true so that the pointer will be freed when StemObject_dispose() is called on it. You can use the stemobject_create_implementation convenience macro as the body of your <Name>_create() function. Multiple create functions that take different parameters are valid; they should all start with <Name>_create and should all have a corresponding function starting with <Name>_init.

		* <Name>_init(): class method which initializes the passed object's fields, including instance method function pointers. Return true if initialization was successful; call ClassName_dispose() and return false if it wasn't.

			* If <Name>_create()/<Name>_init are variadic, there should be variants that take a va_list as their last parameter, named <Name>_vcreate() and <Name>_vinit().

		* <Name>_dispose(): instance method which frees any memory allocated by the object, and frees the object itself if and only if it was created by calling <Name>_create(). The latter is taken care of by StemObject_dispose(), which should be the last thing in the call chain at the end of your <Name>_dispose() function.

	* Instance variable visibility is specified with the private_ivar and protected_ivar macros:

		* Instance variables declared with the private_ivar macro should only be accessed within the implementation of the particular class that declared them.

		* Instance variables declared with the protected_ivar macro should only be accessed within the implementation of the particular class that declared them or one of its subclasses.

		* All other instance variables are considered public and may be accessed by any code anywhere.


Since subclass structs are a semantically different type than their superclasses, calling a superclass method requires a typecast. There are three macros that handle this:

	* Each class's vtable contains function pointers that expect self parameters to be of that class's type. This is done by parameterizing the class name in the MyClass_structContents macro. Your code should look like this:

		#define MyClass_structContents(self_type) \
			MySuperclass_structContents(self_type) \
			void (* method)(self_type * self);
		
		stemobject_struct_definition(MyClass)

	* The compat_type macro can be used to weakly specify the expected type, while evaluating to void *. This of course eliminates compile-type type checks, so it should be used sparingly when possible.

	* The call_super macro provides syntactic sugar and a small degree of extra type safety by convention when calling a superclass method. You must #define SUPERCLASS MySuperclass in your implementation file before using call_super.