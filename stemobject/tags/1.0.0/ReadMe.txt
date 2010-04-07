StemObject serves as the base for Stem's object system. The object system is somewhat informal and done largely by convention. The conventions are enumerated here:

 - Class and instance methods follow the naming convention ClassName_methodName().

 - All instance methods receive, as their first parameter, void * selfPtr. This is the instance on which the method is being called.

 - All instance methods have a corresponding function pointer field in the object struct. These function pointers are set to their corresponding function implementations in *_init().

 - All class header should contain the following line (adjusted for class name) before any #include statements for other Stem objects: typedef struct ClassName ClassName;

 - Object struct contents are #defined as a ClassName_structContents macro in the class's header. Subclasses (which should #define their struct contents in the same way) include their superclass's structContents as the first thing in their own structContents.

 - Method overriding is done by implementing a matching Subclass_method in the subclass, and setting the function pointer in the struct to point to that method in *_init().

 - Calls to superclass instance methods take the form: Superclass_method((Superclass *) self);

 - All subclass objects must call their superclass's *_init() method as the first thing in the subclass's *_init().

 - All subclass objects must call their superclass's *_dispose() method as the last thing in the subclass's *_dispose().

 - All classes must contain the following methods:

  - *_create(): class method which allocates, initializes, and returns an object instance

  - *_init(): class method which initializes the passed object's fields, including instance method function pointers

  - *_dispose(): instance method which frees any memory allocated by the object, and frees the object itself if and only if it was created by calling *_create().